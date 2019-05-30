#include "Antagonist.hpp"
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

Antagonist::Antagonist(GameObject& associated, Vec2 position)
    : Component(associated), position(position) {
  Collider* collider = new Collider(associated);
  Sprite* sprite = new Sprite(associated, ANTAGONIST_SPRITE);
  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Antagonist::~Antagonist() {}

void Antagonist::NotifyCollision(GameObject& other) {}

void Antagonist::Start() {}

void Antagonist::Update(float dt) {
  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;
}

void Antagonist::Render() {}

bool Antagonist::Is(GameData::Types type) const { return type == this->Type; }
