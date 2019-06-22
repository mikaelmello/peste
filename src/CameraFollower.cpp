#include "CameraFollower.hpp"
#include <string>
#include "Camera.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

CameraFollower::CameraFollower(GameObject& associated, Vec2 offset)
    : Component(associated), offset(offset) {}

void CameraFollower::Update(float dt) {
  associated.box.SetCenter(Camera::pos + offset);
}

void CameraFollower::Render() {}

bool CameraFollower::Is(Types type) const { return type == this->Type; }
