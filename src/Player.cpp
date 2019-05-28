#include "Player.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"

#define PLAYER_SPRITE "assets/img/spinning-hope.png"

Player::Player(GameObject& associated) : Component(associated) {
  Sprite* sprite = new Sprite(associated, PLAYER_SPRITE, 8);
  associated.AddComponent(sprite);
  associated.box.x = 500;
  associated.box.y = 500;
}

Player::~Player() {}

void Player::NotifyCollision(GameObject& other) {}

void Player::Start() {}

void Player::Update(float dt) {
  InputManager& input = InputManager::GetInstance();

  if (input.IsKeyDown(UP_ARROW_KEY)) {
    associated.box.y -= dt * 100;
  }
  if (input.IsKeyDown(DOWN_ARROW_KEY)) {
    associated.box.y += dt * 100;
  }
  if (input.IsKeyDown(LEFT_ARROW_KEY)) {
    associated.box.x -= dt * 100;
  }
  if (input.IsKeyDown(RIGHT_ARROW_KEY)) {
    associated.box.x += dt * 100;
  }
}

void Player::Render() {}

bool Player::Is(GameData::Types type) const { return type == this->Type; }
