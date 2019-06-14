#include "PatrolState.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Pathfinder.hpp"

PatrolState::PatrolState(GameObject& antagonist)
    : IState(antagonist), counter(0) {
  Pathfinder::Heuristic* heuristic = new Pathfinder::Diagonal();
  Pathfinder::Astar* pf = new Pathfinder::Astar(
      *heuristic, Game::GetInstance().GetCurrentState().GetCurrentTileMap());

  auto dest1 = Vec2(160, 177);
  auto initial = Game::GetInstance()
                     .GetCurrentState()
                     .GetCurrentTileMap()
                     ->GetInitialPosition();
  auto path1 = pf->Run(initial, dest1);
  patrol_paths.push(path1);

  auto dest2 = Vec2(255, 300);
  auto path2 = pf->Run(dest1, dest2);
  patrol_paths.push(path2);

  timer.Restart();

  delete pf;
}

void PatrolState::OnStateEnter() {
  Pathfinder::Heuristic* heuristic = new Pathfinder::Diagonal();
  Pathfinder::Astar* pf = new Pathfinder::Astar(
      *heuristic, Game::GetInstance().GetCurrentState().GetCurrentTileMap());

  auto initial = Game::GetInstance()
                     .GetCurrentState()
                     .GetCurrentTileMap()
                     ->GetInitialPosition();
  auto current = std::dynamic_pointer_cast<Antagonist>(
                     antagonist.GetComponent(GameData::Antagonist).lock())
                     ->position;

  auto ret_path = pf->Run(current, initial);
  patrol_paths.push(ret_path);

  delete pf;
}

void PatrolState::OnStateExecution() {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      antagonist.GetComponent(GameData::Antagonist).lock());

  auto top = patrol_paths.top();
  ant->position = top[counter];
}

void PatrolState::OnStateExit() {}

void PatrolState::Update(float dt) {
  auto path = patrol_paths.top();

  if (counter == path.size() || path.empty()) {
    if (!path.empty()) {
      patrol_paths.pop();
    }
    OnStateEnter();
    counter = 0;
  }

  if (timer.Get() > 0.1) {
    OnStateExecution();
    timer.Restart();
  }

  timer.Update(dt);
}