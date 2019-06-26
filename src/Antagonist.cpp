#include "Antagonist.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IdleFSM.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"
#include "Player.hpp"
#include "Sprite.hpp"

Antagonist::Antagonist(GameObject& associated, Vec2 position)
    : Component(associated), position(position), stored_state(nullptr) {
  Sprite* sprite = new Sprite(associated, IDLE_SPRITE, 4, 0.125);
  Collider* collider =
      new Collider(associated, {0.6, 0.15}, {0, sprite->GetHeight() * 0.45});

  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Antagonist::~Antagonist() { delete stored_state; }

void Antagonist::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Antagonist::Start() {
  last_direction = Helpers::Direction::NONE;
  state_stack.emplace(new PatrolFSM(associated));
}

void Antagonist::Update(float dt) {
  previous_position = position;

  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("Sprite nao encontrado no Antagonista");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  if (!state_stack.empty()) {
    auto& top_state = state_stack.top();
    if (top_state->PopRequested()) {
      top_state->OnStateExit();
      state_stack.pop();

      if (!state_stack.empty()) {
        auto& enter_state = state_stack.top();
        enter_state->OnStateEnter();
      }
    }

    if (stored_state != nullptr) {
      state_stack.emplace(stored_state);
      stored_state->OnStateEnter();
      stored_state = nullptr;
    }

    if (state_stack.empty()) {
      state_stack.emplace(new PatrolFSM(associated));
    }

    auto& state = state_stack.top();
    state->Update(dt);
  }

  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;

  // a position está no meio horizontal e no fim vertical do sprite
  // para renderizar, colocamos o xy da box de acordo
  // posição * dimensão do tile - (comprimento da sprite / 2), pois o x fica no
  // meio da sprite
  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  // posição * dimensão do tile - altura da sprite, pois o y fica la embaixo
  associated.box.y = position.y * tileDim - sprite->GetHeight();
}

void Antagonist::Render() {}

void Antagonist::Push(IFSM* s) {
  if (stored_state != nullptr) {
    delete stored_state;
  }
  stored_state = s;
}

bool Antagonist::NearTarget() {
  auto player = GameData::PlayerGameObject->GetComponent(PlayerType);
  if (!player) {
    throw std::runtime_error("Player game object without Player component");
  }

  auto playerCp = std::dynamic_pointer_cast<Player>(player);
  double dist = position.Distance(playerCp->position);
  return dist <= ANTAGONIST_DISTANCE;
}

void Antagonist::SpriteManager(Helpers::Action action) {
  switch (action) {
    case Helpers::Action::IDLE:
      break;
    case Helpers::Action::MOVING:
      SpriteMovement();
      break;
    case Helpers::Action::SUSPECTING:
      break;
    case Helpers::Action::CHASING:
      break;
    case Helpers::Action::ATTACKING:
      break;
    default:
      break;
  }
}

void Antagonist::SpriteMovement() {
  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sprite");
  }

  Vec2 delta = position - previous_position;

  bool up = delta.y == -1;
  bool down = delta.y == 1;
  bool left = delta.x == -1;
  bool right = delta.x == 1;

  auto direction = Helpers::combine_moves(up, down, left, right);

  if (last_direction == direction) {
    return;
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  switch (direction) {
    case Helpers::Direction::RIGHT:
      sprite->Open(RIGHT_WALK_SPRITE);
      break;
    case Helpers::Direction::LEFT:
      sprite->Open(LEFT_WALK_SPRITE);
      break;
    case Helpers::Direction::UP:
      sprite->Open(UP_WALK_SPRITE);
      break;
    case Helpers::Direction::DOWN:
      sprite->Open(DOWN_WALK_SPRITE);
      break;
    case Helpers::Direction::UPRIGHT:
      sprite->Open(RIGHT_UP_WALK_SPRITE);
      break;
    case Helpers::Direction::UPLEFT:;
      sprite->Open(LEFT_UP_WALK_SPRITE);
      break;
    case Helpers::Direction::DOWNRIGHT:
      sprite->Open(RIGHT_DOWN_WALK_SPRITE);
      break;
    case Helpers::Direction::DOWNLEFT:
      sprite->Open(LEFT_DOWN_WALK_SPRITE);
      break;
    default:
      sprite->Open(IDLE_SPRITE);  // mudar
      // IdleSpriteManager();
      break;
  }

  last_direction = direction;
}

bool Antagonist::Is(Types type) const { return type == this->Type; }
