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
  Collider* collider = new Collider(associated);
  associated.AddComponent(sprite);

  // just for debugging purposes
  associated.AddComponent(collider);
}

Player::~Player() {}

void Player::NotifyCollision(GameObject& other) {}

void Player::Start() {}

void Player::Update(float dt) {
  InputManager& input = InputManager::GetInstance();

  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();

  std::cout << position.ToString() << ' ' << tilemap->GetLogicalTileDimension()
            << std::endl;
  associated.box.x = position.x * tilemap->GetLogicalTileDimension();
  associated.box.y = position.y * tilemap->GetLogicalTileDimension();

  if (input.IsKeyDown(UP_ARROW_KEY) &&
      tilemap->CanWalk(position.x, position.y - 1)) {
    position.y -= 1;
  }
  if (input.IsKeyDown(DOWN_ARROW_KEY) &&
      tilemap->CanWalk(position.x, position.y + 1)) {
    position.y += 1;
  }
  if (input.IsKeyDown(LEFT_ARROW_KEY) &&
      tilemap->CanWalk(position.x - 1, position.y)) {
    position.x -= 1;
  }
  if (input.IsKeyDown(RIGHT_ARROW_KEY) &&
      tilemap->CanWalk(position.x + 1, position.y)) {
    position.x += 1;
  }
}

void Player::Render() {}

bool Player::Is(GameData::Types type) const { return type == this->Type; }
