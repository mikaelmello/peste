#include "CameraFollower.hpp"
#include <string>
#include "Camera.hpp"
#include "GameData.hpp"

CameraFollower::CameraFollower(GameObject& associated)
    : Component(associated) {}

void CameraFollower::Update(float dt) {
  associated.box.x = Camera::pos.x;
  associated.box.y = Camera::pos.y;
}

void CameraFollower::Render() {}

bool CameraFollower::Is(GameData::Types type) const {
  return type == this->Type;
}
