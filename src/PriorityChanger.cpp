#include "PriorityChanger.hpp"
#include <algorithm>
#include <memory>
#include <string>
#include "Collider.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

PriorityChanger::PriorityChanger(GameObject& associated, GameObject& object,
                                 PCType type)
    : Component(associated), object(object), type(type) {
  Collider* collider = new Collider(associated);
  associated.AddComponent(collider);
}

PriorityChanger::~PriorityChanger() {}

void PriorityChanger::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto pcComponent = other->GetComponent(PriorityChangerType);
  if (pcComponent) {
    currentCollision.push_back(other.get());
  }
}

void PriorityChanger::SetRect(float dt, Rect rect) {
  auto colliderCpt = associated.GetComponent(ColliderType);
  if (!colliderCpt) {
    throw std::runtime_error("PC sem collider");
  }
  associated.box = rect;
  colliderCpt->Update(dt);
}

void PriorityChanger::Start() {}

void PriorityChanger::Update(float dt) {
  if (currentCollision.size() == 0) {
    return;
  }

  if (type == PCPlayer) {
    printf("pooooooorra\n");
  }

  currentCollision.push_back(&associated);

  std::sort(currentCollision.begin(), currentCollision.end(), GameObjectComp());
  double curPriority = 15;
  for (int i = currentCollision.size() - 1; i >= 0; i--) {
    auto object = currentCollision[i]->GetComponent(PriorityChangerType);
    auto pc = std::dynamic_pointer_cast<PriorityChanger>(object);
    pc->object.SetPriority(curPriority);
    curPriority -= 1;
  }

  currentCollision.clear();
}

void PriorityChanger::Render() {}

bool PriorityChanger::Is(Types type) const { return type == this->Type; }
