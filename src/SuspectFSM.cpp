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
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a SuspectFSM em "
        "StateEnter");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

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
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a SuspectFSM em "
        "StateExecution");
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
  Pathfinder::Astar pf = Pathfinder::Astar(
      object, Game::GetInstance().GetCurrentState().GetCurrentTileMap());

  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a SuspectFSM em "
        "StateExit");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  Vec2 current = ant->position;

  auto return_path = pf.Run(current, initial);
  PatrolFSM::patrol_paths.emplace(0, return_path);
}

void SuspectFSM::Update(float dt) {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a SuspectFSM em "
        "Update");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);
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
