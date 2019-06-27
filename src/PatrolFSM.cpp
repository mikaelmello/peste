#include "PatrolFSM.hpp"

#include <utility>
#include "Antagonist.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IdleFSM.hpp"
#include "Pathfinder.hpp"
#include "SuspectFSM.hpp"
#include "Types.hpp"

std::stack<std::pair<int, std::vector<Vec2>>> PatrolFSM::patrol_paths;

PatrolFSM::PatrolFSM(GameObject& object) : IFSM(object) {}

PatrolFSM::~PatrolFSM() {}

void PatrolFSM::OnStateEnter() {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  if (patrol_paths.empty()) {
    Pathfinder::Astar* pf = new Pathfinder::Astar(object, tilemap);

    auto dest1 = Vec2(100, 192);
    auto dest2 = Vec2(100, 142);
    auto dest3 = Vec2(200, 142);

    auto initial = tilemap->GetInitialPosition();

    auto ant = object.GetComponent(Types::AntagonistType);

    if (!ant) {
      throw std::runtime_error("No antagonist component in antagonist_go");
    }

    Vec2 current = std::dynamic_pointer_cast<Antagonist>(ant)->position;

    try {
      auto path3 = pf->Run(dest2, dest3);
      patrol_paths.emplace(0, path3);

      auto path2 = pf->Run(dest1, dest2);
      patrol_paths.emplace(0, path2);

      auto path1 = pf->Run(initial, dest1);
      patrol_paths.emplace(0, path1);

      auto ret_path = pf->Run(current, initial);
      patrol_paths.emplace(0, ret_path);
    } catch (const std::exception& ex) {
      printf("%s\n", ex.what());
      patrol_paths = std::stack<std::pair<int, std::vector<Vec2>>>();
      pop_requested = true;
    }

    delete pf;
  }
}

void PatrolFSM::OnStateExecution() {
  if (!patrol_paths.empty()) {
    auto antCpt = object.GetComponent(AntagonistType);
    if (!antCpt) {
      throw std::runtime_error(
          "Nao tem antagonista no objeto passado para a PatrolFSM");
    }
    auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

    int& k = patrol_paths.top().first;
    std::vector<Vec2>& top = patrol_paths.top().second;
    ant->position = top[k];
    k++;

    ant->AssetsManager(Helpers::Action::MOVING);
  }
}

void PatrolFSM::OnStateExit() {
  auto ant_cpt = object.GetComponent(SpriteType);
  if (!ant_cpt) {
    throw std::runtime_error(
        "O gameobject na patrol fsm em on state exit nao tem antagonist");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);

  ant->AssetsManager(Helpers::Action::IDLE);
}

void PatrolFSM::Update(float dt) {
  auto ant_cpt = object.GetComponent(Types::AntagonistType);

  if (!ant_cpt) {
    throw std::runtime_error("no antagonist component in antagonist_go");
  }

  auto ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);

  OnStateEnter();

  bool enter = false;
  while (!patrol_paths.empty() &&
         patrol_paths.top().first == patrol_paths.top().second.size()) {
    patrol_paths.pop();
    enter = true;
  }

  if (enter) {
    ant->Push(new IdleFSM(object));
  }

  if (ant->NearTarget()) {
    ant->Push(new SuspectFSM(object));
  }

  OnStateExecution();
}
