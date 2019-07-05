#include "SuspectFSM.hpp"

#include "Antagonist.hpp"
#include "Game.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"
#include "Player.hpp"
#include "PursuitFSM.hpp"
#include "Types.hpp"

SuspectFSM::SuspectFSM(GameObject& object) : IFSM(object), rage_bias(0) {
  stack_original_size = Antagonist::paths.size();
}

SuspectFSM::~SuspectFSM() {}

void SuspectFSM::OnStateEnter() {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonist em SuspectFSM::OnStateEnter");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (rage_bias == RAGE_NUMERIC_LIMIT) rage_bias--;

  if (ant->NearTarget(50)) {
    Game& game = Game::GetInstance();
    State& state = game.GetCurrentState();
    auto tilemap = state.GetCurrentTileMap();

    Walkable w = GetWalkable(object, *GameData::PlayerGameObject);

    if (w.can_walk) {
      auto pf = Pathfinder::Astar(object, tilemap);

      initial = ant->position;
      Antagonist::paths.emplace(0, pf.Run(initial, w.walkable));

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

  unsigned& k = Antagonist::paths.top().first;
  std::vector<Vec2>& path = Antagonist::paths.top().second;

  if (k < path.size()) {
    ant->position = path[k];
  }

  if (rage_bias == RAGE_NUMERIC_LIMIT) {
    ant->Push(new PursuitFSM(object));
  }

  ant->AssetsManager(Helpers::Action::SUSPECTING);
}

void SuspectFSM::OnStateExit() {
  while (stack_original_size < Antagonist::paths.size()) {
    Antagonist::paths.pop();
  }

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
  Antagonist::paths.emplace(0, return_path);
}

void SuspectFSM::Update(float dt) {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonist em SuspectFSM::Update");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (bias_update_timer.Get() > 3 * dt) {
    if (ant->NearTarget(50))
      rage_bias = std::min(rage_bias + 1, RAGE_NUMERIC_LIMIT);
    else
      rage_bias = std::max(rage_bias - 1, NO_RAGE_BIAS);
    bias_update_timer.Restart();
  }

  OnStateExecution();

  if (rage_bias == NO_RAGE_BIAS) {
    if (pop_request_timer.Get() >= POP_REQUEST_TIME)
      pop_requested = true;
    else
      pop_request_timer.Update(dt);

  } else {
    pop_request_timer.Restart();
  }

  unsigned& k = Antagonist::paths.top().first;
  std::vector<Vec2>& path = Antagonist::paths.top().second;

  if (k < path.size() - 1) {
    k++;
  }

  bias_update_timer.Update(dt);
}
