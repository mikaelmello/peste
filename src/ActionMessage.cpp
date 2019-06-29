#include <iostream>
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

ActionMessage::ActionMessage(GameObject& associated, Vec2 position,
                             const std::string& file)
    : Component(associated) {
  Sprite* sprite = new Sprite(associated, file);
  associated.AddComponent(sprite);

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8 - 30;
}

ActionMessage::~ActionMessage() {}

void ActionMessage::NotifyCollision(std::shared_ptr<GameObject> other) {}

void ActionMessage::Start() {}

void ActionMessage::Update(float dt) {}

void ActionMessage::Render() {}

bool ActionMessage::Is(Types type) const { return type == this->Type; }
