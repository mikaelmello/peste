#include "Camera.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Vec2.hpp"

GameObject* Camera::focus;
Vec2 Camera::pos;
Vec2 Camera::speed = {300, 0};

void Camera::Follow(GameObject* newFocus) { focus = newFocus; }

void Camera::Unfollow() { focus = nullptr; }

void Camera::Update(float dt) {
  if (focus == nullptr) {
    return;
  }

  Game& game = Game::GetInstance();
  pos = focus->box.Center() - Vec2(game.GetWidth() / 2, game.GetHeight() / 2);
}
