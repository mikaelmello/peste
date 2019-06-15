#include "Antagonist.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Pathfinder.hpp"
#include "PatrolState.hpp"
#include "Sprite.hpp"

Antagonist::Antagonist(GameObject& associated, Vec2 position)
    : Component(associated), position(position) {
  Collider* collider = new Collider(associated);
  Sprite* sprite = new Sprite(associated, ANTAGONIST_SPRITE, 4, 0.1);

  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Antagonist::~Antagonist() {}

void Antagonist::NotifyCollision(GameObject& other) {}

void Antagonist::Start() {
  position = Vec2(240, 192);
  state.push(new PatrolState(associated));
}

void Antagonist::Update(float dt) {
  if (!state.empty()) {
    if (state.top()->PopRequested()) {
      state.pop();
      state.top()->OnStateEnter();
    }
  } else {
    state.push(new PatrolState(associated));
  }

  if (!state.empty()) {
    auto p = (PatrolState*)state.top();
    p->Update(dt);
  }

  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;
}

void Antagonist::Render() {}

bool Antagonist::Is(GameData::Types type) const { return type == this->Type; }
