#include "Antagonist.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Pathfinder.hpp"
#include "Sprite.hpp"

Antagonist::Antagonist(GameObject& associated, Vec2 position)
    : Component(associated), position(position), i(0) {
  Collider* collider = new Collider(associated);
  Sprite* sprite = new Sprite(associated, ANTAGONIST_SPRITE, 4, 0.1);
  associated.AddComponent(collider);
  associated.AddComponent(sprite);

  timer1.Restart();
  timer2.Restart();
}

Antagonist::~Antagonist() {}

void Antagonist::NotifyCollision(GameObject& other) {}

void Antagonist::Start() {}

void Antagonist::Update(float dt) {
  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;

  if (timer1.Get() < 0.1) {
    auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
    auto a_star = new Pathfinder::Astar(new Pathfinder::Manhattan(), tilemap);

    Vec2 b = {140, 177};
    auto c = a_star->Run(position, b);
    path.assign(c.begin(), c.end());
  }
  timer1.Update(dt);

  if (timer2.Get() > 0.01) {
    if (path.size() != 0) {
      position = path[i];
      i = (i + 1) % path.size();
      timer2.Restart();
    }
  }

  timer2.Update(dt);
}

void Antagonist::Render() {}

bool Antagonist::Is(GameData::Types type) const { return type == this->Type; }
