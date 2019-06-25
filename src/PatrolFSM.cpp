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

PatrolFSM::PatrolFSM(GameObject& object)
    : IFSM(object), sprite_status(IDLE_CODE) {}

PatrolFSM::~PatrolFSM() {}

void PatrolFSM::OnStateEnter() {
  if (patrol_paths.empty()) {
    Pathfinder::Astar* pf = new Pathfinder::Astar(
        object, Game::GetInstance().GetCurrentState().GetCurrentTileMap());

    auto dest1 = Vec2(100, 192);
    auto dest2 = Vec2(100, 142);
    auto dest3 = Vec2(200, 142);

    auto initial = Game::GetInstance()
                       .GetCurrentState()
                       .GetCurrentTileMap()
                       ->GetInitialPosition();

    initial = Vec2(426, 160);
    auto current = std::dynamic_pointer_cast<Antagonist>(
                       object.GetComponent(AntagonistType).lock())
                       ->position;

    try {
      auto path3 = pf->Run(dest2, dest3);
      patrol_paths.emplace(0, path3);

      auto path2 = pf->Run(dest1, dest2);
      patrol_paths.emplace(0, path2);

      auto path1 = pf->Run(initial, dest1);
      patrol_paths.emplace(0, path1);

      auto ret_path = pf->Run(current, initial);
      patrol_paths.emplace(0, ret_path);
    } catch (...) {
      pop_requested = true;
    }

    delete pf;
  }
}

void PatrolFSM::OnStateExecution() {
  if (!patrol_paths.empty()) {
    auto ant = std::dynamic_pointer_cast<Antagonist>(
        object.GetComponent(AntagonistType).lock());
    std::pair<int, int> previous = {ant->position.x, ant->position.y};

    int& k = patrol_paths.top().first;
    std::vector<Vec2>& top = patrol_paths.top().second;
    ant->position = top[k];
    k++;

    // Isso será usado em pelo menos 3 estados. Repensar.
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
          object.GetComponent(SpriteType).lock());
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
  // Isso será usado em pelo menos 3 estados. Repensar.
}

void PatrolFSM::OnStateExit() {
  auto sprite =
      std::dynamic_pointer_cast<Sprite>(object.GetComponent(SpriteType).lock());
  sprite_status = IDLE_CODE;
  sprite->Open(IDLE_SPRITE);
}

void PatrolFSM::Update(float dt) {
  auto ant = std::dynamic_pointer_cast<Antagonist>(
      object.GetComponent(AntagonistType).lock());

  OnStateEnter();

  while (!patrol_paths.empty() &&
         patrol_paths.top().first == patrol_paths.top().second.size()) {
    patrol_paths.pop();

    ant->Push(new IdleFSM(object));
  }

  if (ant->NearTarget()) {
    ant->Push(new SuspectFSM(object));
  }

  OnStateExecution();
}
