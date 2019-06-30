#include "Blocker.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

Blocker::Blocker(GameObject& associated, Vec2 scale, Vec2 offset)
    : Component(associated), block(true), colliding(false) {
  Collider* collider = new Collider(associated, scale, offset);
  associated.AddComponent(collider);

  tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  beginX = associated.box.x / tileDim;
  endX = (associated.box.x + associated.box.w) / tileDim + 1;
  beginY = associated.box.y / tileDim;
  endY = (associated.box.y + associated.box.h) / tileDim + 1;

  printf("%d %d %d %d\n", beginX, endX, beginY, endY);

  for (int i = beginX; i < endX; i++) {
    for (int j = beginY; j < endY; j++) {
      tilemap->SetWalk(i, j, !block);
    }
  }
}

Blocker::~Blocker() {}

void Blocker::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }
}

void Blocker::Start() {}

void Blocker::Update(float dt) {}

void Blocker::Render() {}

bool Blocker::Is(Types type) const { return type == this->Type; }

void Blocker::Block() {
  // if (!colliding) {
  for (int i = beginX; i < endX; i++) {
    for (int j = beginY; j < endY; j++) {
      tilemap->SetWalk(i, j, false);
    }
  }
  block = true;
  // }
  colliding = false;
}

void Blocker::Unblock() {
  for (int i = beginX; i < endX; i++) {
    for (int j = beginY; j < endY; j++) {
      tilemap->SetWalk(i, j, true);
    }
  }
}

bool Blocker::IsBlocking() const { return block; }
