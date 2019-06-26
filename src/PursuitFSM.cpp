#include "PursuitFSM.hpp"
#include "Antagonist.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Types.hpp"

PursuitFSM::PursuitFSM(GameObject& object) : IFSM(object) {
  auto tile_map = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  pf = std::unique_ptr<Pathfinder::Astar>(
      new Pathfinder::Astar(object, tile_map));
}

PursuitFSM::~PursuitFSM() {}

void PursuitFSM::OnStateEnter() {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a PursuitFSM");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  auto player = GameData::PlayerGameObject->GetComponent(PlayerType);
  if (!player) {
    throw std::runtime_error("Player game object without Player component");
  }

  auto playerCp = std::dynamic_pointer_cast<Player>(player);
  auto pursuit_path = pf->Run(ant->position, playerCp->position);
  path = {0, pursuit_path};
}

void PursuitFSM::OnStateExecution() {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a PursuitFSM em Execution");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (path.first < path.second.size()) {
    ant->position = path.second[path.first];
  }
}

void PursuitFSM::OnStateExit() {}

void PursuitFSM::Update(float dt) {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a PursuitFSM em Update");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (timer.Get() >= 1) {
    OnStateEnter();
    timer.Restart();
  }
  OnStateExecution();

  if (path.first < path.second.size() - 1) {
    path.first++;
  }

  if (!ant->NearTarget()) {
    pop_requested = true;
  }

  timer.Update(dt);
}
