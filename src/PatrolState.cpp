#include "PatrolState.hpp"
#include "Antagonist.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Pathfinder.hpp"

PatrolState::PatrolState(GameObject& antagonist)
    : IState(antagonist), counter(0) {
  OnStateEnter();
  timer.Restart();
}

PatrolState::~PatrolState() {}

void PatrolState::OnStateEnter() {
  Pathfinder::Heuristic* heuristic = new Pathfinder::Diagonal();
  Pathfinder::Astar* pf = new Pathfinder::Astar(
      *heuristic, Game::GetInstance().GetCurrentState().GetCurrentTileMap());

  auto dest1 = Vec2(160, 177);
  auto dest2 = Vec2(240, 200);

  auto initial = Game::GetInstance()
                     .GetCurrentState()
                     .GetCurrentTileMap()
                     ->GetInitialPosition();
  auto path1 = pf->Run(dest1, dest2);
  patrol_paths.push(path1);

  auto path2 = pf->Run(initial, dest1);
  patrol_paths.push(path2);

  auto current = std::dynamic_pointer_cast<Antagonist>(
                     antagonist.GetComponent(GameData::Antagonist).lock())
                     ->position;
  auto ret_path = pf->Run(current, initial);
  patrol_paths.push(ret_path);

  timer.Restart();

  delete pf;
}

void PatrolState::OnStateExecution() {
  if (!patrol_paths.empty()) {
    auto ant = std::dynamic_pointer_cast<Antagonist>(
        antagonist.GetComponent(GameData::Antagonist).lock());

    auto top = patrol_paths.top();

    ant->position = top[counter];
    counter++;
  }
}

void PatrolState::OnStateExit() {}

void PatrolState::Update(float dt) {
  if (patrol_paths.empty()) {
    OnStateEnter();
    counter = 0;
  }

  auto path = patrol_paths.top();

  if (counter == path.size()) {
    patrol_paths.pop();
    counter = 0;
  }

  if (timer.Get() > 0.01) {
    OnStateExecution();
    timer.Restart();
  }

  timer.Update(dt);
}