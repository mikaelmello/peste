#include "Terry.hpp"
#include <memory>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

#define TERRY_IDLE_SPRITE "assets/img/npc/npc.png"

Terry::Terry(GameObject& associated, Vec2 position)
    : Component(associated), position(position) {
  Sprite* sprite = new Sprite(associated, TERRY_IDLE_SPRITE);
  Collider* collider =
      new Collider(associated, {0.5, 0.3}, {0, sprite->GetHeight() * 0.35f});
  associated.AddComponent(collider);
  associated.AddComponent(sprite);
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  associated.box.y = position.y * tileDim - sprite->GetHeight();
}

Terry::~Terry() {}

void Terry::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Terry::Start() {}

void Terry::Update(float dt) {}

void Terry::Render() {}

bool Terry::Is(Types type) const { return type == this->Type; }
