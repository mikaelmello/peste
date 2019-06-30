#include "Blocker.hpp"
#include <memory>
#include <string>
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

Blocker::Blocker(GameObject& associated, Vec2 scale, Vec2 offset)
    : Component(associated), block(true), colliding(false) {
  Collider* collider = new Collider(associated, scale, offset);
  associated.AddComponent(collider);
}

Blocker::~Blocker() {}

void Blocker::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }
}

void Blocker::Start() {}

void Blocker::Update(float dt) {}

void Blocker::Render() {}

bool Blocker::Is(Types type) const { return type == this->Type; }

void Blocker::Block() {
  if (!colliding) {
    block = true;
  }
  colliding = false;
}

void Blocker::Unblock() { block = false; }

bool Blocker::IsBlocking() const { return block; }
