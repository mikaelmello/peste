#include "PursuitFSM.hpp"

#include "Antagonist.hpp"
#include "Game.hpp"

PursuitFSM::PursuitFSM(GameObject& object) : IFSM(object) {
  auto heuristic = new Pathfinder::Diagonal();
  auto tile_map = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  pf = std::unique_ptr<Pathfinder::Astar>(
      new Pathfinder::Astar(object, heuristic, tile_map));
  OnStateEnter();
}

PursuitFSM::~PursuitFSM() {}

void PursuitFSM::OnStateEnter() {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(GameData::Antagonist).lock());

  Vec2 hope_position_test(100, 177);

  auto pursuit_path = pf->Run(ant->position, hope_position_test);
  path = {0, pursuit_path};
}

void PursuitFSM::OnStateExecution() {
  printf("hey\n");

  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(GameData::Antagonist).lock());

  ant->position = path.second[path.first];
}

void PursuitFSM::OnStateExit() {}

void PursuitFSM::Update(float dt) {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(GameData::Antagonist).lock());

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