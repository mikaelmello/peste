#include "Terry.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "PriorityChanger.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "Types.hpp"

#define TERRY_IDLE_SPRITE "assets/img/npc/npc.png"

Terry::Terry(GameObject& associated, Vec2 position)
    : Component(associated), position(position), colliding(false) {
  Sprite* sprite = new Sprite(associated, TERRY_IDLE_SPRITE);
  Collider* collider = new Collider(associated, {1.6, 1.6}, {0.8, 0.8});
  associated.AddComponent(sprite);
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
  associated.AddComponent(collider);

  std::cout << position.x << " " << position.y << std::endl;

  State& state = Game::GetInstance().GetCurrentState();

  auto tilemap = state.GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;

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

  GameObject* talkGo = new GameObject();
  ActionMessage* talkMsg =
      new ActionMessage(*talkGo, position, "assets/img/open_msg.png");
  talkGo->AddComponent(talkMsg);
  talkMessageGo = state.AddObject(talkGo);
}

Terry::~Terry() {
  blocker_go->RequestDelete();
  priorityChanger_go->RequestDelete();
  talkMessageGo->RequestDelete();
}

void Terry::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }
}

void Terry::Start() {}

void Terry::Update(float dt) {}

void Terry::Render() {
  if (colliding) {
    ShowTalkDialog();
  } else {
    HideTalkDialog();
  }
  colliding = false;
}

void Terry::ShowTalkDialog() { talkMessageGo->EnableRender(); }

void Terry::HideTalkDialog() { talkMessageGo->DisableRender(); }

bool Terry::Is(Types type) const { return type == this->Type; }
