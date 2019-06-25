#include "PickupItem.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

PickupItem::PickupItem(GameObject& associated, Vec2 position)
    : Component(associated) {
  Sprite* sprite = new Sprite(associated, "assets/img/x.png");
  associated.AddComponent(sprite);

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8 - 30;
}

PickupItem::~PickupItem() {}

void PickupItem::NotifyCollision(std::shared_ptr<GameObject> other) {}

void PickupItem::Start() {}

void PickupItem::Update(float dt) {}

void PickupItem::Render() {}

bool PickupItem::Is(Types type) const { return type == this->Type; }
