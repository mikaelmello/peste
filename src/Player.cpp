#include "Player.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

using namespace Helpers;

#define PLAYER_FRONT_ANIM "assets/img/hope/front_anim.png"
#define PLAYER_BACK_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_LEFT_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_RIGHT_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_UPLEFT_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_DOWNLEFT_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_UPRIGHT_ANIM "assets/img/hope/back_anim.png"
#define PLAYER_DOWNRIGHT_ANIM "assets/img/hope/back_anim.png"

#define PLAYER_FRONT "assets/img/hope/front.png"
#define PLAYER_BACK "assets/img/hope/back.png"
#define PLAYER_LEFT "assets/img/hope/left.png"
#define PLAYER_RIGHT "assets/img/hope/right.png"
#define PLAYER_UPLEFT "assets/img/hope/leftup.png"
#define PLAYER_UPRIGHT "assets/img/hope/rightup.png"
#define PLAYER_DOWNLEFT "assets/img/hope/leftdown.png"
#define PLAYER_DOWNRIGHT "assets/img/hope/rightdown.png"

Player::Player(GameObject& associated, Vec2 position)
    : Component(associated), position(position) {
  Sprite* sprite = new Sprite(associated, PLAYER_FRONT);
  sprite->SetScaleX(0.7, 0.7);
  Collider* collider = new Collider(associated);
  associated.AddComponent(sprite);
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
  // just for debugging purposes
  associated.AddComponent(collider);
}

Player::~Player() {}

void Player::NotifyCollision(GameObject& other) {}

void Player::Start() {}

void Player::Update(float dt) {
  InputManager& input = InputManager::GetInstance();
  Vec2 oldPos(position.x, position.y);
  bool canwalk = true;
  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  auto cpt = associated.GetComponent(GameData::Types::Sprite);
  auto sprite = std::dynamic_pointer_cast<Sprite>(cpt.lock());

  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool changed = false;

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;

  int w = associated.box.w / tileDim;
  int h = associated.box.h / tileDim;
  int x = position.x + w / 4;
  int y = position.y + h - tileDim / 4;

  canwalk = true;
  if (input.IsKeyDown(UP_ARROW_KEY)) {
    up = true;
    for (int i = 0; i < w / 2 && canwalk; i++) {
      canwalk = tilemap->CanWalk(x + i, y - 1);
    }
    if (canwalk) {
      position.y -= 1;
    }
  }
  if (input.IsKeyDown(DOWN_ARROW_KEY)) {
    down = true;
    for (int i = 0; i < w / 2 && canwalk; i++) {
      canwalk = tilemap->CanWalk(x + i, y + tileDim / 4 + 1);
    }
    if (canwalk) {
      position.y += 1;
    }
  }
  if (input.IsKeyDown(LEFT_ARROW_KEY)) {
    left = true;
    for (int i = 0; i < tileDim / 4 && canwalk; i++) {
      canwalk = tilemap->CanWalk(x - 1, y + i);
    }
    if (canwalk) {
      position.x -= 1;
    }
  }
  if (input.IsKeyDown(RIGHT_ARROW_KEY)) {
    right = true;
    for (int i = 0; i < tileDim / 4 && canwalk; i++) {
      canwalk = tilemap->CanWalk(x + w / 2 + 1, y + i);
    }
    if (canwalk) {
      position.x += 1;
    }
  }

  if (!up && !right && !left && !down) {
    IDLEAnimation(sprite);
  } else {
    changed = WalkingAnimation(sprite, combine_moves(up, down, left, right));
    if (changed) {
      sprite->SetFrameCount(5);
      sprite->SetFrameTime(0.1);
    }
  }

  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
}

void Player::Render() {}

bool Player::Is(GameData::Types type) const { return type == this->Type; }

void Player::IDLEAnimation(const std::shared_ptr<Sprite>& sprite) {
  sprite->SetFrameCount(1);
  sprite->SetFrameTime(1);

  switch (lastMove) {
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
      sprite->Open(PLAYER_UPLEFT);
      break;
    case Direction::UPRIGHT:
      sprite->Open(PLAYER_UPRIGHT);
      break;
    case Direction::DOWNLEFT:
      sprite->Open(PLAYER_DOWNLEFT);
      break;
    case Direction::DOWNRIGHT:
      sprite->Open(PLAYER_DOWNRIGHT);
      break;
  }

  lastMove = Direction::NONE;
}

bool Player::WalkingAnimation(const std::shared_ptr<Sprite>& sprite,
                              Direction direction) {
  if (lastMove == direction) {
    return false;
  }

  switch (direction) {
    case Direction::UP:
      sprite->Open(PLAYER_BACK_ANIM);
      break;
    case Direction::DOWN:
      sprite->Open(PLAYER_FRONT_ANIM);
      break;
    case Direction::LEFT:
      sprite->Open(PLAYER_LEFT_ANIM);
      break;
    case Direction::RIGHT:
      sprite->Open(PLAYER_RIGHT_ANIM);
      break;
    case Direction::UPLEFT:
      sprite->Open(PLAYER_UPLEFT_ANIM);
      break;
    case Direction::UPRIGHT:
      sprite->Open(PLAYER_UPRIGHT_ANIM);
      break;
    case Direction::DOWNLEFT:
      sprite->Open(PLAYER_DOWNLEFT_ANIM);
      break;
    case Direction::DOWNRIGHT:
      sprite->Open(PLAYER_DOWNRIGHT_ANIM);
      break;
  }

  lastMove = direction;
  return true;
}
