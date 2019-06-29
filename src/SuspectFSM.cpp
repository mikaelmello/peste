#include "SuspectFSM.hpp"

#include "Antagonist.hpp"
#include "Game.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"
#include "Player.hpp"
#include "PursuitFSM.hpp"
#include "Types.hpp"

SuspectFSM::SuspectFSM(GameObject& object) : IFSM(object), rage_bias(0) {}

SuspectFSM::~SuspectFSM() {}

void SuspectFSM::OnStateEnter() {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonist em SuspectFSM::OnStateEnter");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (ant->NearTarget()) {
    Game& game = Game::GetInstance();
    State& state = game.GetCurrentState();
    auto tilemap = state.GetCurrentTileMap();

    Walkable w = GetWalkable(object, *GameData::PlayerGameObject);

    if (w.can_walk) {
      auto pf = Pathfinder::Astar(object, tilemap);

      initial = ant->position;
      path = {0, pf.Run(initial, w.walkable)};

    } else {
      pop_requested = true;
    }
  }
}

void SuspectFSM::OnStateExecution() {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonist em SuspectFSM::OnStateExecution");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (path.first < path.second.size()) {
    ant->position = path.second[path.first];
  }

  if (rage_bias == RAGE_NUMERIC_LIMIT) {
    ant->Push(new PursuitFSM(object));
  }

  ant->AssetsManager(Helpers::Action::SUSPECTING);
}

void SuspectFSM::OnStateExit() {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  auto pf = Pathfinder::Astar(object, tilemap);

  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonist em SuspectFSM::OnStateExit");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  Vec2 current = ant->position;

  auto return_path = pf.Run(current, initial);
  PatrolFSM::patrol_paths.emplace(0, return_path);
}

void SuspectFSM::Update(float dt) {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonist em SuspectFSM::Update");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);
  OnStateExecution();

  if (bias_update_timer.Get() > 3 * dt) {
    if (ant->NearTarget())
      rage_bias = std::min(rage_bias + 1, RAGE_NUMERIC_LIMIT);
    else
      rage_bias = std::max(rage_bias - 1, NO_RAGE_BIAS);
    bias_update_timer.Restart();
  }

  if (rage_bias == NO_RAGE_BIAS) {
    if (pop_request_timer.Get() >= POP_REQUEST_TIME)
      pop_requested = true;
    else
      pop_request_timer.Update(dt);

  } else {
    pop_request_timer.Restart();
  }

  if (path.first < path.second.size() - 1) {
    path.first++;
  }

  bias_update_timer.Update(dt);
}
