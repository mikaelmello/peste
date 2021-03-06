#include "GameObject.hpp"
#include <algorithm>
#include <memory>
#include <vector>
#include "Component.hpp"
#include "Game.hpp"

GameObject::GameObject(double priority)
    : angleDeg(0),
      priority(priority),
      isDead(false),
      started(false),
      render(true) {}

GameObject::~GameObject() {}

void GameObject::Start() {
  started = true;
  std::for_each(components.begin(), components.end(),
                [](std::shared_ptr<Component>& cpt) { cpt->Start(); });
}

void GameObject::Update(float dt) {
  std::for_each(components.rbegin(), components.rend(),
                [&](std::shared_ptr<Component>& cpt) { cpt->Update(dt); });
}

void GameObject::NotifyCollision(std::shared_ptr<GameObject> other) {
  std::for_each(
      components.begin(), components.end(),
      [&](std::shared_ptr<Component>& cpt) { cpt->NotifyCollision(other); });
}

void GameObject::Render() {
  if (!render) return;

  std::for_each(components.begin(), components.end(),
                [](std::shared_ptr<Component>& cpt) { cpt->Render(); });
}

bool GameObject::IsRendering() { return render; }

bool GameObject::IsDead() const { return isDead; }

void GameObject::RequestDelete() { isDead = true; }

void GameObject::ReverseDelete() { isDead = false; }

void GameObject::AddComponent(Component* cpt) {
  components.emplace_back(cpt);

  if (started) {
    cpt->Start();
  }
}

void GameObject::RemoveComponent(Component* cpt) {
  auto it = std::find_if(
      components.begin(), components.end(),
      [&](std::shared_ptr<Component>& cpt2) { return cpt2.get() == cpt; });

  if (it != components.end()) {
    components.erase(it);
  }
}

std::shared_ptr<Component> GameObject::GetComponent(Types type) {
  auto it = std::find_if(
      components.begin(), components.end(),
      [&](std::shared_ptr<Component>& cpt2) { return cpt2->Is(type); });

  if (it == components.end()) {
    return std::shared_ptr<Component>();
  }

  return *it;
}

void GameObject::SetPriority(double newPriority) {
  this->priority = newPriority;
}
