#include "SuspectFSM.hpp"

#include "Antagonist.hpp"
#include "Game.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"

SuspectFSM::SuspectFSM(GameObject& object) : IFSM(object), rage_bias(0) {
  OnStateEnter();
}

SuspectFSM::~SuspectFSM() {}

void SuspectFSM::OnStateEnter() {
  Pathfinder::Heuristic* heuristic = new Pathfinder::Diagonal();
  Pathfinder::Astar* pf = new Pathfinder::Astar(
      object, heuristic,
      Game::GetInstance().GetCurrentState().GetCurrentTileMap());

  initial = std::dynamic_pointer_cast<Antagonist>(
                object.GetComponent(GameData::Antagonist).lock())
                ->position;
  Vec2 verify_position(200, 142);

  path = {0, pf->Run(initial, verify_position)};

  delete pf;
}

void SuspectFSM::OnStateExecution() {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(GameData::Antagonist).lock());

  ant->position = path.second[path.first];
  if (rage_bias == RAGE_NUMERIC_LIMIT) {
    // printf("Entrou em modo perseguição.");
    // ant->Push(new RageFSM(object));
  }
  // Sprite Manager.
  // Sprite Manager.
}

void SuspectFSM::OnStateExit() {
  Pathfinder::Heuristic* heuristic = new Pathfinder::Diagonal();
  Pathfinder::Astar* pf = new Pathfinder::Astar(
      object, heuristic,
      Game::GetInstance().GetCurrentState().GetCurrentTileMap());

  Vec2 test_pos(200, 142);
  auto return_path = pf->Run(test_pos, initial);

  PatrolFSM::patrol_paths.emplace(0, return_path);
  delete pf;
}

void SuspectFSM::Update(float dt) {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(GameData::Antagonist).lock());
  OnStateExecution();

  if (bias_update_timer.Get() > dt) {
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
  } else {  // else absolutamente temporário.
    pop_requested = true;
  }

  bias_update_timer.Update(dt);
}
