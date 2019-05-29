#include "Player.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

#define PLAYER_SPRITE "assets/img/spinning-hope.png"

Player::Player(GameObject& associated, Vec2 position)
    : Component(associated), position(position) {
  Sprite* sprite = new Sprite(associated, PLAYER_SPRITE, 8);
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

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;

  int w = associated.box.w / tileDim;
  int h = associated.box.h / tileDim;
  int x = position.x + w / 4;
  int y = position.y + h - tileDim / 4;

  if (input.IsKeyDown(UP_ARROW_KEY)) {
    canwalk = true;
    for (int i = 0; i < w / 2 && canwalk; i++) {
      canwalk = canwalk = tilemap->CanWalk(x + i, y - 1);
    }
    if (canwalk) {
      position.y -= 1;
    }
  }
  if (input.IsKeyDown(DOWN_ARROW_KEY)) {
    canwalk = true;
    for (int i = 0; i < w / 2 && canwalk; i++) {
      canwalk = canwalk = tilemap->CanWalk(x + i, y + tileDim / 4 + 1);
    }
    if (canwalk) {
      position.y += 1;
    }
  }
  if (input.IsKeyDown(LEFT_ARROW_KEY)) {
    canwalk = true;
    for (int i = 0; i < tileDim / 4 && canwalk; i++) {
      canwalk = canwalk = tilemap->CanWalk(x - 1, y + i);
    }
    if (canwalk) {
      position.x -= 1;
    }
  }
  if (input.IsKeyDown(RIGHT_ARROW_KEY)) {
    canwalk = true;
    for (int i = 0; i < tileDim / 4 && canwalk; i++) {
      canwalk = canwalk = tilemap->CanWalk(x + w / 2 + 1, y + i);
    }
    if (canwalk) {
      position.x += 1;
    }
  }
}

void Player::Render() {}

bool Player::Is(GameData::Types type) const { return type == this->Type; }
