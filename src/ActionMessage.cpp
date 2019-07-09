#include "ActionMessage.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

ActionMessage::ActionMessage(GameObject& associated, Vec2 position,
                             const std::string& file)
    : Component(associated) {
  Sprite* sprite = new Sprite(associated, file);
  associated.AddComponent(sprite);

  auto spriteCpt = associated.GetComponent(SpriteType);
  sprite_ptr = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8 - 30;
}

ActionMessage::~ActionMessage() {}

void ActionMessage::NotifyCollision(std::shared_ptr<GameObject> other) {}

void ActionMessage::Start() {}

void ActionMessage::Update(float dt) {}

void ActionMessage::Render() {}

void ActionMessage::UpdateSprite(const std::string& file) {
  sprite_ptr->Open(file);
}

bool ActionMessage::Is(Types type) const { return type == this->Type; }
