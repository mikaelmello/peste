#include "CameraFollower.hpp"
#include <string>
#include "Camera.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

CameraFollower::CameraFollower(GameObject& associated, Vec2 offset)
    : Component(associated), offset(offset) {}

void CameraFollower::Update(float dt) {
  auto newPos = Camera::pos + offset;
  associated.box.x = newPos.x;
  associated.box.y = newPos.y;
}

void CameraFollower::Render() {}

bool CameraFollower::Is(Types type) const { return type == this->Type; }
