#include "PatrolFSM.hpp"

#include <utility>
#include "Antagonist.hpp"
#include "Component.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IdleFSM.hpp"
#include "Pathfinder.hpp"

PatrolFSM::PatrolFSM(GameObject& antagonist)
    : IFSM(antagonist), counter(0), sprite_status(IDLE_CODE) {
  OnStateEnter();
  timer.Restart();
}

PatrolFSM::~PatrolFSM() {}

void PatrolFSM::OnStateEnter() {
  if (!patrol_paths.empty()) {
    // Garantir que sempre inicia este método com a pilha vazia.
    // Checar se essa linha de código não dá problema.
    patrol_paths = std::stack<std::vector<Vec2>>();
  }

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

void PatrolFSM::OnStateExecution() {
  bool change_sprite = false;
  int last_sprite = 0;

  if (!patrol_paths.empty()) {
    auto ant = std::dynamic_pointer_cast<Antagonist>(
        antagonist.GetComponent(GameData::Antagonist).lock());
    std::pair<int, int> previous = {ant->position.x, ant->position.y};

    std::vector<Vec2> top = patrol_paths.top();
    ant->position = top[counter];
    counter++;

    std::pair<int, int> delta = {ant->position.x - previous.first,
                                 ant->position.y - previous.second};

    std::vector<std::pair<int, int>> deltas = {{-1, 0}, {1, 0},  {0, 1},
                                               {0, -1}, {-1, 1}, {-1, -1},
                                               {1, 1},  {1, -1}, {0, 0}};

    int sprite_code;
    auto it = std::find(deltas.begin(), deltas.end(), delta);

    if (it != deltas.end()) {
      sprite_code = std::distance(deltas.begin(), it);
    } else {
      sprite_code = IDLE_CODE;
    }

    if (sprite_code != sprite_status) {
      auto sprite = std::dynamic_pointer_cast<Sprite>(
          antagonist.GetComponent(GameData::Sprite).lock());
      std::string sprite_path;
      switch (sprite_code) {
        case LEFT_WALK_CODE:
          sprite_path = LEFT_WALK_SPRITE;
          break;
        case RIGHT_WALK_CODE:
          sprite_path = RIGHT_WALK_SPRITE;
          break;
        case DOWN_WALK_CODE:
          sprite_path = DOWN_WALK_SPRITE;
          break;
        case UP_WALK_CODE:
          sprite_path = UP_WALK_SPRITE;
          break;
        case LEFT_DOWN_WALK_CODE:
          sprite_path = LEFT_DOWN_WALK_SPRITE;
          break;
        case LEFT_UP_WALK_CODE:
          sprite_path = LEFT_UP_WALK_SPRITE;
          break;
        case RIGHT_DOWN_WALK_CODE:
          sprite_path = RIGHT_DOWN_WALK_SPRITE;
          break;
        case RIGHT_UP_WALK_CODE:
          sprite_path = RIGHT_UP_WALK_SPRITE;
          break;
        default:
          sprite_path = IDLE_SPRITE;
          sprite_code = IDLE_CODE;
          break;
      }
      sprite->Open(sprite_path);
      sprite_status = sprite_code;
    }
  }
}

void PatrolFSM::OnStateExit() {}

void PatrolFSM::Update(float dt) {
  if (patrol_paths.empty()) {
    OnStateEnter();
    counter = 0;
  }

  auto path = patrol_paths.top();

  if (counter == path.size()) {
    patrol_paths.pop();
    counter = 0;
  }

  if (timer.Get() > 0.03) {
    OnStateExecution();
    timer.Restart();
  }

  timer.Update(dt);
}