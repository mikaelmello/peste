#include "CameraAction.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "Timer.hpp"
#include "Vec2.hpp"

bool CameraAction::isRunning = false;
float CameraAction::speed = 1500;
float CameraAction::angle;
float CameraAction::distanceLeft;
Timer CameraAction::timer;

void CameraAction::Start(Vec2 source, Vec2 target) {
  auto& game = Game::GetInstance();
  isRunning = true;
  distanceLeft = source.Distance(target);
  angle = (target - source).XAxisInclination();
  Camera::Unfollow();

  auto centerScreen = Vec2(game.GetWidth() / 2, game.GetHeight() / 2);
  source -= centerScreen;
  target -= centerScreen;

  Camera::pos = source;
}

void CameraAction::Update(float dt) {
  if (distanceLeft == 0) {
    timer.Update(dt);
    if (timer.Get() > 3) {
      Stop();
    }
    return;
  }
  auto distanceToMove = speed * dt;
  if (distanceLeft <= distanceToMove) {
    auto movement = Vec2(distanceLeft, 0).GetRotated(angle);
    Camera::pos += movement;
    distanceLeft = 0;
    timer.Restart();
    return;
  }

  distanceLeft -= distanceToMove;

  auto movement = Vec2(distanceToMove, 0).GetRotated(angle);
  Camera::pos += movement;
}

bool CameraAction::IsRunning() { return isRunning; }

void CameraAction::Stop() {
  Camera::Follow(GameData::PlayerGameObject.get());
  isRunning = false;
}
