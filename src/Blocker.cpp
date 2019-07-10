#include "Blocker.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

Blocker::Blocker(GameObject& associated, Vec2 scale, Vec2 offset)
    : Component(associated), block(true) {
  Collider* collider = new Collider(associated, scale, offset);
  associated.AddComponent(collider);

  int tileDim = 8;

  Rect box = associated.box;
  box.w *= scale.x;
  box.h *= scale.y;
  box.SetCenter(associated.box.Center());

  box += offset.GetRotated(Helpers::deg_to_rad(associated.angleDeg));

  beginX = box.x / tileDim;
  endX = (box.x + box.w) / tileDim + 1;
  beginY = box.y / tileDim;
  endY = (box.y + box.h) / tileDim + 1;

  for (int i = beginX; i < endX; i++) {
    for (int j = beginY; j < endY; j++) {
      tilemap->SetWalk(i, j, false);
    }
  }
}

Blocker::~Blocker() {}

void Blocker::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Blocker::Start() {}

void Blocker::Update(float dt) {}

void Blocker::Render() {}

bool Blocker::Is(Types type) const { return type == this->Type; }

void Blocker::Block() {
  for (int i = beginX; i < endX; i++) {
    for (int j = beginY; j < endY; j++) {
      tilemap->SetWalk(i, j, false);
    }
  }
  block = true;
}

void Blocker::Unblock() {
  for (int i = beginX; i < endX; i++) {
    for (int j = beginY; j < endY; j++) {
      tilemap->SetWalk(i, j, true);
    }
  }
  block = false;
}

bool Blocker::IsBlocking() const { return block; }
