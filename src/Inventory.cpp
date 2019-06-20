#include "Inventory.hpp"
#include <string>
#include "CameraFollower.hpp"
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

Inventory::Inventory(GameObject& associated) : Component(associated) {
  Sprite* sprite = new Sprite(associated, "assets/img/inventory-temp.png");
  CameraFollower* cf = new CameraFollower(associated);
  associated.AddComponent(sprite);
  associated.AddComponent(cf);

  associated.box.SetCenter({512, 384});
}

Inventory::~Inventory() {}

void Inventory::NotifyCollision(GameObject& other) {}

void Inventory::Start() {}

void Inventory::Update(float dt) { printf("Uai\n"); }

void Inventory::Render() { printf("ta?\n"); }

bool Inventory::Is(GameData::Types type) const { return type == this->Type; }
