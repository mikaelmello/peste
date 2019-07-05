#include "PursuitFSM.hpp"
#include "Antagonist.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Types.hpp"

PursuitFSM::PursuitFSM(GameObject& object) : IFSM(object) {
  auto antagonist_cpt = object.GetComponent(AntagonistType);
  if (!antagonist_cpt) {
    throw std::runtime_error("sem antagonista em PursuitFSM::OnStateEnter");
  }

  auto tile_map = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  pf = std::unique_ptr<Pathfinder::Astar>(
      new Pathfinder::Astar(object, tile_map));

  auto ant = std::dynamic_pointer_cast<Antagonist>(antagonist_cpt);

  stack_original_size = Antagonist::paths.size();
  initial = ant->position;
  OnStateEnter();
}

PursuitFSM::~PursuitFSM() {}

void PursuitFSM::OnStateEnter() {
  auto antagonist_cpt = object.GetComponent(AntagonistType);
  if (!antagonist_cpt) {
    throw std::runtime_error("sem antagonista em PursuitFSM::OnStateEnter");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antagonist_cpt);

  Walkable w = GetWalkable(object, *GameData::PlayerGameObject);

  if (w.can_walk) {
    try {
      auto pursuit_path = pf->Run(ant->position, w.walkable);
      Antagonist::paths.emplace(0, pursuit_path);

    } catch (const std::exception& ex) {
      printf("%s\n", ex.what());
      pop_requested = true;
    }

  } else {
    pop_requested = true;
  }
}

void PursuitFSM::OnStateExecution() {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonista em PursuitFSM::Execution");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  unsigned& k = Antagonist::paths.top().first;
  std::vector<Vec2>& path = Antagonist::paths.top().second;

  if (k < path.size()) {
    ant->position = path[k];
  }

  ant->AssetsManager(Helpers::Action::CHASING);
}

void PursuitFSM::OnStateExit() {
  while (stack_original_size < Antagonist::paths.size()) {
    Antagonist::paths.pop();
  }

  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonista em PursuitFSM::OnStateExit");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  auto return_path = pf->Run(ant->position, initial);
  Antagonist::paths.emplace(0, return_path);
}

void PursuitFSM::Update(float dt) {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonista em PursuitFSM::Update");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (timer.Get() >= 1.0f) {
    if (stack_original_size < Antagonist::paths.size()) {
      Antagonist::paths.pop();
    }
    OnStateEnter();
    timer.Restart();
  }

  OnStateExecution();
  unsigned& k = Antagonist::paths.top().first;
  std::vector<Vec2>& path = Antagonist::paths.top().second;

  if (k < path.size() - 1) {
    k++;
  }

  if (!ant->NearTarget(40)) {
    pop_requested = true;
  }

  timer.Update(dt);
}
