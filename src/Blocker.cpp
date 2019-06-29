#include "Blocker.hpp"
#include <memory>
#include <string>
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

Blocker::Blocker(GameObject& associated, Vec2 scale, Vec2 offset)
    : Component(associated) {
  Collider* collider = new Collider(associated, scale, offset);
  associated.AddComponent(collider);
}

Blocker::~Blocker() {}

void Blocker::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Blocker::Start() {}

void Blocker::Update(float dt) {}

void Blocker::Render() {}

bool Blocker::Is(Types type) const { return type == this->Type; }
