#include "Player.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Blocker.hpp"
#include "Camera.hpp"
#include "Collider.hpp"
#include "Door.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Item.hpp"
#include "PriorityChanger.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "Types.hpp"

using namespace Helpers;

#define PLAYER_FRONT_ANIM "assets/img/hope/front_anim.png"
#define PLAYER_BACK_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_LEFT_ANIM "assets/img/hope/left_anim.png"
#define PLAYER_RIGHT_ANIM "assets/img/hope/right_anim.png"

#define PLAYER_FRONT "assets/img/hope/front.png"
#define PLAYER_BACK "assets/img/hope/back.png"
#define PLAYER_LEFT "assets/img/hope/left.png"
#define PLAYER_RIGHT "assets/img/hope/right.png"

Player::Player(GameObject& associated, Vec2 position)
    : Component(associated),
      position(position),
      lastDirection(Helpers::Direction::NONE) {
  Sprite* sprite = new Sprite(associated, PLAYER_FRONT);
  Collider* collider =
      new Collider(associated, {0.5, 0.3}, {0, sprite->GetHeight() * 0.35f});
  associated.AddComponent(sprite);
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
  // just for debugging purposes
  associated.AddComponent(collider);

  State& state = Game::GetInstance().GetCurrentState();

  GameObject* pcGo = new GameObject(associated.priority);
  pcGo->box = associated.box;
  PriorityChanger* priChanger = new PriorityChanger(*pcGo, associated, true);
  pcGo->AddComponent(priChanger);
  priorityChanger_go = state.AddObject(pcGo);
}

Player::~Player() { priorityChanger_go->RequestDelete(); }

void Player::NotifyCollision(std::shared_ptr<GameObject> other) {
  InputManager& input = InputManager::GetInstance();
  auto item_cpt = other->GetComponent(ItemType);

  if (item_cpt) {
    if (input.KeyPress(X_KEY)) {
      GameData::AddToInventory(other);
    }
  }

  auto door_cpt = other->GetComponent(DoorType);
  if (door_cpt) {
    auto door = std::dynamic_pointer_cast<Door>(door_cpt);
    if (input.KeyPress(X_KEY)) {
      if (door->IsOpen())
        door->Close();
      else
        door->Open();
    }
  }
}

void Player::Start() {}

void Player::Update(float dt) {
  Vec2 oldPos(position.x, position.y);
  InputManager& input = InputManager::GetInstance();
  bool canwalk = true;
  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  auto spriteCpt = associated.GetComponent(SpriteType);
  auto colliderCpt = associated.GetComponent(ColliderType);

  if (!spriteCpt || !colliderCpt) {
    throw std::runtime_error("Nao tem sprite nem collider no player");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  auto collider = std::dynamic_pointer_cast<Collider>(colliderCpt);

  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;

  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  associated.box.y = position.y * tileDim - sprite->GetHeight();

  int w = round(collider->box.w / tileDim);  // comprimento da box em celulas
  if (w % 2 == 1) w--;
  int h = round(collider->box.h / tileDim);  // altura da box em

  // inicio da "box de colisao" imaginaria com celulas
  int x = position.x - w / 2;
  int y = position.y - h;

  if (input.IsKeyDown(UP_ARROW_KEY)) {
    canwalk = true;
    up = true;
    for (int i = 0; i < w && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x + i, y - 1);
    }
    if (canwalk) {
      position.y -= 1;
    }
  }
  if (input.IsKeyDown(DOWN_ARROW_KEY)) {
    canwalk = true;
    down = true;
    for (int i = 0; i < w && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x + i, y + h + 1);
    }
    if (canwalk) {
      position.y += 1;
    }
  }
  if (input.IsKeyDown(LEFT_ARROW_KEY)) {
    canwalk = true;
    left = true;
    for (int i = 0; i < h && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x - 1, y + i);
    }
    if (canwalk) {
      position.x -= 1;
    }
  }
  if (input.IsKeyDown(RIGHT_ARROW_KEY)) {
    canwalk = true;
    right = true;
    for (int i = 0; i < h && canwalk; i++) {
      canwalk &= tilemap->CanWalk(x + w + 1, y + i);
    }
    if (canwalk) {
      position.x += 1;
    }
  }

  auto direction = combine_moves(up, down, left, right);

  if (direction == Direction::NONE) {
    OpenIdleSprite(sprite, lastDirection);
  } else if (lastDirection != direction) {
    OpenWalkingSprite(sprite, direction);
  }
  lastDirection = direction;

  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  priorityChanger_go->box = associated.box;
}

void Player::Render() {
#if DEBUG
  Vec2 point = Vec2(0, 0) - Camera::pos;
  SDL_Point points[5];
  points[0] = {(int)point.x, (int)point.y};
  points[4] = {(int)point.x, (int)point.y};

  point = (Vec2(1024, 0)) - Camera::pos;
  points[1] = {(int)point.x, (int)point.y};

  point = (Vec2(1024, 768)) - Camera::pos;
  points[2] = {(int)point.x, (int)point.y};

  point = (Vec2(0, 768)) - Camera::pos;
  points[3] = {(int)point.x, (int)point.y};

  SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 0,
                         SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif
}

bool Player::Is(Types type) const { return type == this->Type; }

void Player::OpenIdleSprite(const std::shared_ptr<Sprite>& sprite,
                            Direction lastDirection) {
  sprite->SetFrameCount(1);
  sprite->SetFrameTime(1);

  switch (lastDirection) {
    case Direction::UP:
      sprite->Open(PLAYER_BACK);
      break;
    case Direction::DOWN:
      sprite->Open(PLAYER_FRONT);
      break;
    case Direction::LEFT:
      sprite->Open(PLAYER_LEFT);
      break;
    case Direction::RIGHT:
      sprite->Open(PLAYER_RIGHT);
      break;
    case Direction::UPLEFT:
      sprite->Open(PLAYER_BACK);
      break;
    case Direction::UPRIGHT:
      sprite->Open(PLAYER_BACK);
      break;
    case Direction::DOWNLEFT:
      sprite->Open(PLAYER_FRONT);
      break;
    case Direction::DOWNRIGHT:
      sprite->Open(PLAYER_FRONT);
      break;
    case Direction::NONE:
      sprite->Open(PLAYER_FRONT);
      break;
  }
}

void Player::OpenWalkingSprite(const std::shared_ptr<Sprite>& sprite,
                               Direction direction) {
  switch (direction) {
    case Direction::UP:
      sprite->Open(PLAYER_BACK_ANIM);
      sprite->SetFrameCount(5);
      break;
    case Direction::DOWN:
      sprite->Open(PLAYER_FRONT_ANIM);
      sprite->SetFrameCount(5);
      break;
    case Direction::LEFT:
      sprite->Open(PLAYER_LEFT_ANIM);
      sprite->SetFrameCount(8);
      break;
    case Direction::RIGHT:
      sprite->Open(PLAYER_RIGHT_ANIM);
      sprite->SetFrameCount(8);
      break;
    case Direction::UPLEFT:
      sprite->Open(PLAYER_BACK_ANIM);
      sprite->SetFrameCount(5);
      break;
    case Direction::UPRIGHT:
      sprite->Open(PLAYER_BACK_ANIM);
      sprite->SetFrameCount(5);
      break;
    case Direction::DOWNLEFT:
      sprite->Open(PLAYER_FRONT_ANIM);
      sprite->SetFrameCount(5);
      break;
    case Direction::DOWNRIGHT:
      sprite->Open(PLAYER_FRONT_ANIM);
      sprite->SetFrameCount(5);
      break;
    default:
      return OpenIdleSprite(sprite, direction);
  }
  sprite->SetFrameTime(0.05);
}
