#include "PriorityChanger.hpp"
#include <memory>
#include <string>
#include "Collider.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

PriorityChanger::PriorityChanger(GameObject& associated, GameObject& object,
                                 bool player)
    : Component(associated), object(object), player(player) {
  Collider* collider = new Collider(associated);
  associated.AddComponent(collider);
}

PriorityChanger::~PriorityChanger() {}

void PriorityChanger::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto pcComponent = other->GetComponent(PriorityChangerType);
  if (pcComponent && !player) {
    if (other->box.y + other->box.h < object.box.y + object.box.h) {
      object.SetPriority(other->priority + 1);
    } else {
      object.SetPriority(other->priority - 1);
    }
  }
}

void PriorityChanger::Start() {}

void PriorityChanger::Update(float dt) {}

void PriorityChanger::Render() {}

bool PriorityChanger::Is(Types type) const { return type == this->Type; }
