#include "Terry.hpp"
#include <memory>
#include <string>
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "PriorityChanger.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"

#define TERRY_IDLE_SPRITE "assets/img/npc/npc.png"

Terry::Terry(GameObject& associated, Vec2 position)
    : Component(associated), position(position) {
  Sprite* sprite = new Sprite(associated, TERRY_IDLE_SPRITE);
  Collider* collider =
      new Collider(associated, {0.5, 0.3f}, {0, sprite->GetHeight() * 0.35f});
  associated.AddComponent(collider);
  associated.AddComponent(sprite);
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  State& state = Game::GetInstance().GetCurrentState();

  auto tilemap = state.GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  associated.box.y = position.y * tileDim - sprite->GetHeight();

  GameObject* bgo = new GameObject(associated.priority);
  bgo->box = associated.box;
  Blocker* blocker =
      new Blocker(*bgo, {0.5, 0.3f}, {0, sprite->GetHeight() * 0.35f});
  bgo->AddComponent(blocker);
  blocker_go = state.AddObject(bgo);

  GameObject* pcGo = new GameObject(associated.priority);
  pcGo->box = associated.box;
  PriorityChanger* priChanger = new PriorityChanger(*pcGo, associated);
  pcGo->AddComponent(priChanger);
  priorityChanger_go = state.AddObject(pcGo);
}

Terry::~Terry() {
  blocker_go->RequestDelete();
  priorityChanger_go->RequestDelete();
}

void Terry::NotifyCollision(std::shared_ptr<GameObject> other) {
  // add new state with dialogue if hope press x when colliding with terry
}

void Terry::Start() {}

void Terry::Update(float dt) {}

void Terry::Render() {}

bool Terry::Is(Types type) const { return type == this->Type; }
