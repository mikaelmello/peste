#include "Item.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Camera.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Player.hpp"
#include "RoomState.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "TutorialState.hpp"
#include "Types.hpp"

Item::Item(GameObject& associated, const std::string& name,
           const std::string& description, const std::string& spritePath,
           Vec2 pos)
    : Component(associated),
      name(name),
      description(description),
      position(pos),
      colliding(false) {
  Sprite* sprite = new Sprite(associated, spritePath);
  sprite->SetScaleX(0.07, 0.07);

  Collider* collider =
      new Collider(associated, {1, 0.3}, {0, sprite->GetHeight() / 2.5});
  associated.AddComponent(collider);
  associated.AddComponent(sprite);

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  GameObject* pickupGo = new GameObject(associated.priority);
  ActionMessage* pickup =
      new ActionMessage(*pickupGo, position, "assets/img/x.png");
  pickupGo->AddComponent(pickup);

  State& state = Game::GetInstance().GetCurrentState();
  pickupItemGo = state.AddObject(pickupGo);

  GameObject* blocker_go = new GameObject(associated.priority);
  Blocker* blocker =
      new Blocker(*blocker_go, {1, 0.3}, {0, sprite->GetHeight() / 2.5});
  blocker->Unblock();
  blocker_go->box = associated.box;
  blocker_go->AddComponent(blocker);
  state.AddObject(blocker_go);
}

Item::~Item() {}

void Item::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }
}

void Item::Start() {}

void Item::Update(float dt) {}

void Item::Render() {
  if (colliding) {
    ShowPickupDialog();
  } else {
    HidePickupDialog();
  }
  colliding = false;
}

void Item::Pickup() { pickupItemGo->RequestDelete(); }

void Item::HidePickupDialog() { pickupItemGo->DisableRender(); }

void Item::ShowPickupDialog() { pickupItemGo->EnableRender(); }

bool Item::Is(Types type) const { return type == this->Type; }

void Item::SetCenter(Vec2 pos) { associated.box.SetCenter(pos); }

void Item::SetScale(float scaleX, float scaleY) {
  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("Nao tem sprite no Item ao setar a escala");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  sprite->SetScaleX(scaleX, scaleY);
}
