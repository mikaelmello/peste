#include "Item.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

Item::Item(GameObject& associated, const std::string& name,
           const std::string& description, const std::string& spritePath,
           Vec2 pos)
    : Component(associated),
      name(name),
      description(description),
      position(pos) {
  Sprite* sprite = new Sprite(associated, spritePath);
  sprite->SetScaleX(0.07, 0.07);
  Collider* collider =
      new Collider(associated, {1, 0.3}, {0, sprite->GetHeight() / 2.5});
  associated.AddComponent(collider);
  associated.AddComponent(sprite);
  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
}

Item::~Item() {}

void Item::NotifyCollision(GameObject& other) {}

void Item::Start() {}

void Item::Update(float dt) {}

void Item::Render() {}

bool Item::Is(GameData::Types type) const { return type == this->Type; }

void Item::SetCenter(Vec2 pos) { associated.box.SetCenter(pos); }

void Item::SetScale(float scaleX, float scaleY) {
  auto sprite = associated.GetComponent(GameData::Types::Sprite);
  auto spriteSharedPtr = std::dynamic_pointer_cast<Sprite>(sprite.lock());

  spriteSharedPtr->SetScaleX(scaleX, scaleY);
}
