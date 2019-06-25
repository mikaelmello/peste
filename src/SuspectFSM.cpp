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
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(AntagonistType));

  if (ant->NearTarget()) {
    Pathfinder::Astar* pf = new Pathfinder::Astar(
        object, Game::GetInstance().GetCurrentState().GetCurrentTileMap());

    initial = ant->position;

    auto player = GameData::PlayerGameObject->GetComponent(PlayerType);
    if (!player) {
      throw std::runtime_error("Player game object without Player component");
    }

    auto playerCp = std::dynamic_pointer_cast<Player>(player);
    path = {0, pf->Run(initial, playerCp->position)};

    delete pf;
  }
}

void SuspectFSM::OnStateExecution() {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(AntagonistType));

  if (path.first < path.second.size()) {
    ant->position = path.second[path.first];
  }

  if (rage_bias == RAGE_NUMERIC_LIMIT) {
    ant->Push(new PursuitFSM(object));
  }
}

void SuspectFSM::OnStateExit() {
  Pathfinder::Astar* pf = new Pathfinder::Astar(
      object, Game::GetInstance().GetCurrentState().GetCurrentTileMap());

  Vec2 current = std::dynamic_pointer_cast<Antagonist>(
                     object.GetComponent(AntagonistType))
                     ->position;

  auto return_path = pf->Run(current, initial);
  PatrolFSM::patrol_paths.emplace(0, return_path);

  delete pf;
}

void SuspectFSM::Update(float dt) {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(AntagonistType));
  OnStateExecution();

  if (bias_update_timer.Get() > 3 * dt) {
    if (ant->NearTarget()) {
      rage_bias = std::min(rage_bias + 1, RAGE_NUMERIC_LIMIT);
    } else {
      rage_bias = std::max(rage_bias - 1, NO_RAGE_BIAS);
    }
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
