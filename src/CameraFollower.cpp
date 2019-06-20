#include "CameraFollower.hpp"
#include <string>
#include "Camera.hpp"
#include "GameData.hpp"
#include "Vec2.hpp"

CameraFollower::CameraFollower(GameObject& associated, Vec2 offset)
    : Component(associated), offset(offset) {}

void CameraFollower::Update(float dt) {
  associated.box.SetCenter(Camera::pos + offset);
}

void CameraFollower::Render() {}

bool CameraFollower::Is(GameData::Types type) const {
  return type == this->Type;
}
