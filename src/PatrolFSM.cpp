#include "PatrolFSM.hpp"

#include <utility>
#include "Antagonist.hpp"
#include "Collider.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IdleFSM.hpp"
#include "Pathfinder.hpp"
#include "SuspectFSM.hpp"
#include "Types.hpp"

PatrolFSM::PatrolFSM(GameObject& object, std::vector<Vec2>& points)
    : IFSM(object) {
  auto ant_cpt = object.GetComponent(Types::AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("sem antagonist in PatrolFSM::PatrolFSM");
  }
  ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);

  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  try {
    Pathfinder::Astar pf = Pathfinder::Astar(object, tilemap);
    int n = points.size();

    for (int i = n - 1; i >= 0; i--) {
      auto path = pf.Run(points[i], points[(i + 1) % n]);
      paths.emplace(0, path);
    }
  } catch (const std::exception& ex) {
    printf("%s\n", ex.what());
  }
}

PatrolFSM::~PatrolFSM() {}

void PatrolFSM::OnStateEnter() {
  if (Antagonist::paths.empty()) {
    Antagonist::paths = paths;
  }
}

void PatrolFSM::OnStateExecution(float dt) {
  printf("On state execution do patrol\n");
  bool must_pop = UpdatePosition(dt);
  if (must_pop) {
    printf("Idle???\n");
    Antagonist::paths.pop();
    ant.lock()->Push(new IdleFSM(object));
  }
  ant.lock()->AssetsManager(Helpers::Action::MOVING);
}

void PatrolFSM::OnStateExit() {
  ant.lock()->AssetsManager(Helpers::Action::IDLE);
}

void PatrolFSM::Update(float dt) {
  printf("Patrulando.\n");
  OnStateEnter();
  OnStateExecution(dt);

  if (ant.lock()->NearTarget(50)) {
    ant.lock()->Push(new SuspectFSM(object));
  }
}
