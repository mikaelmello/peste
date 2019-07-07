#include "PursuitFSM.hpp"
#include "Antagonist.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Types.hpp"

PursuitFSM::PursuitFSM(GameObject& object) : IFSM(object) {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  auto antagonist_cpt = object.GetComponent(AntagonistType);
  if (!antagonist_cpt) {
    throw std::runtime_error("sem antagonista em PursuitFSM::PursuitFSM");
  }
  ant = std::dynamic_pointer_cast<Antagonist>(antagonist_cpt);

  auto pf_ptr = new Pathfinder::Astar(object, tilemap);
  pf = std::unique_ptr<Pathfinder::Astar>(pf_ptr);

  stack_original_size = Antagonist::paths.size();
  initial = ant.lock()->position;
}

PursuitFSM::~PursuitFSM() {}

void PursuitFSM::OnStateEnter() {
  Walkable w = GetWalkable(*GameData::PlayerGameObject);

  if (w.can_walk) {
    try {
      auto pursuit_path = pf->Run(ant.lock()->position, w.walkable);
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
  bool finished = UpdatePosition();
  if (finished) {
    OnStateEnter();
  }
  ant.lock()->AssetsManager(Helpers::Action::CHASING);
}

void PursuitFSM::OnStateExit() {
  while (stack_original_size < Antagonist::paths.size()) {
    Antagonist::paths.pop();
  }

  auto return_path = pf->Run(ant.lock()->position, initial);
  Antagonist::paths.emplace(0, return_path);
}

void PursuitFSM::Update(float dt) {
  if (timer.Get() >= 0.75f) {
    Antagonist::paths.pop();
    OnStateEnter();
    timer.Restart();
  }

  OnStateExecution();
  pop_requested = !ant.lock()->NearTarget(40);
  timer.Update(dt);
}
