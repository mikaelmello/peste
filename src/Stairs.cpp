#include "Stairs.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "PriorityChanger.hpp"
#include "Sprite.hpp"

#define DOWN_STAIR "assets/img/stairs/down.png"
#define UP_STAIR "assets/img/stairs/up.png"

Stairs::Stairs(GameObject& associated, Helpers::Direction direction,
               Vec2 position, Helpers::Floor floor)
    : Component(associated), colliding(false) {
  Sprite* sprite = new Sprite(associated);
  Collider* collider = new Collider(associated);

  State& state = Game::GetInstance().GetCurrentState();

  GameObject* actMsgGo = new GameObject();
  ActionMessage* actMsg;

  if (direction == Helpers::Direction::DOWN) {
    sprite->Open(DOWN_STAIR);
    collider->SetScale({1, 0.3});
    collider->SetOffset({0, sprite->GetHeight() * 0.35f});

    actMsg = new ActionMessage(*actMsgGo, position + Vec2(-10, 60),
                               "assets/img/goUp.png");
  } else if (direction == Helpers::Direction::UP) {
    sprite->Open(UP_STAIR);
    collider->SetScale({1, 1.2});
    collider->SetOffset({0, 0.6});

    actMsg = new ActionMessage(*actMsgGo, position, "assets/img/goDown.png");
  } else {
    throw std::runtime_error("Ta errada a direction da escada irmao.\n");
  }

  actMsgGo->AddComponent(actMsg);
  actionMessageGo = state.AddObject((actMsgGo));

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  GameObject* blocker_go = new GameObject(associated.priority);
  blocker_go->box = associated.box;
  Blocker* blocker;

  GameObject* pcGo = new GameObject(associated.priority);
  pcGo->box = associated.box;
  PriorityChanger* priChanger = new PriorityChanger(*pcGo, associated);
  pcGo->AddComponent(priChanger);
  state.AddObject(pcGo);

  if (direction == Helpers::Direction::DOWN) {
    blocker =
        new Blocker(*blocker_go, {1, 0.25}, {0, sprite->GetHeight() * 0.30f});
  } else if (direction == Helpers::Direction::UP) {
    blocker = new Blocker(*blocker_go, {1, 0.5}, {0, 0.25f});
  }

  blocker_go->AddComponent(blocker);
  state.AddObject(blocker_go);

  associated.AddComponent(sprite);
  associated.AddComponent(collider);
}

Stairs::~Stairs() {}

void Stairs::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }
}

void Stairs::Start() {}

void Stairs::Update(float dt) {}

void Stairs::Render() {
  if (colliding) {
    ShowInteractDialog();
  } else {
    HideInteractDialog();
  }
  colliding = false;
}

void Stairs::ShowInteractDialog() { actionMessageGo->EnableRender(); }

void Stairs::HideInteractDialog() { actionMessageGo->DisableRender(); }

bool Stairs::Is(Types type) const { return type == this->Type; }
