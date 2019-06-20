#include "Item.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

Item::Item(GameObject& associated, const std::string& file, Vec2 pos)
    : Component(associated) {
  Sprite* sprite = new Sprite(associated, file);
  sprite->SetScaleX(0.07, 0.07);
  Collider* collider =
      new Collider(associated, {1, 0.3}, {0, sprite->GetHeight() / 2.5});
  associated.AddComponent(collider);
  associated.AddComponent(sprite);
  associated.box.x = pos.x * 8;
  associated.box.y = pos.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
}

Item::~Item() {}

void Item::NotifyCollision(GameObject& other) {}

void Item::Start() {}

void Item::Update(float dt) {}

void Item::Render() {}

bool Item::Is(GameData::Types type) const { return type == this->Type; }
