#include "Door.hpp"
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
#include "State.hpp"

#define LEFT_DOOR_CLOSED "assets/img/doors/door_left_closed.png"
#define LEFT_DOOR_OPEN "assets/img/doors/door_left_open.png"
#define RIGHT_DOOR_CLOSED "assets/img/doors/door_right_closed.png"
#define RIGHT_DOOR_OPEN "assets/img/doors/door_right_open.png"
#define FRONT_DOOR_CLOSED "assets/img/doors/door_front_closed.png"
#define FRONT_DOOR_OPEN "assets/img/doors/door_front_open.png"
#define DOUBLE_DOOR_CLOSED "assets/img/doors/doubleDoor_front_closed.png"
#define DOUBLE_DOOR_OPEN "assets/img/doors/doubleDoor_front_open.png"

Door::Door(GameObject& associated, Helpers::Direction direction, Vec2 position,
           bool open, bool doubledoor)
    : Component(associated), position(position), colliding(false), open(open) {
  Sprite* sprite = new Sprite(associated);

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  Collider* collider = new Collider(associated);

  State& state = Game::GetInstance().GetCurrentState();

  GameObject* blocker_go = new GameObject(associated.priority);
  blocker_go->box = associated.box;
  Blocker* blocker = new Blocker(*blocker_go);

  if (direction == Helpers::Direction::LEFT) {
    closePath = LEFT_DOOR_CLOSED;
    openPath = LEFT_DOOR_OPEN;
    sprite->Open(LEFT_DOOR_CLOSED);
    collider->SetScale({4, 1});
    collider->SetOffset({1, 0.75});
  } else if (direction == Helpers::Direction::RIGHT) {
    // blocker = new Blocker(*blocker_go);
    closePath = RIGHT_DOOR_CLOSED;
    openPath = RIGHT_DOOR_OPEN;
    sprite->Open(RIGHT_DOOR_CLOSED);
    collider->SetScale({4, 1});
    collider->SetOffset({1, 0.75});
  } else if (direction == Helpers::Direction::UP) {
    // blocker = new Blocker(*blocker_go, {1, 0.3});
    if (doubledoor) {
      closePath = DOUBLE_DOOR_CLOSED;
      openPath = DOUBLE_DOOR_OPEN;
      sprite->Open(DOUBLE_DOOR_CLOSED);
    } else {
      closePath = FRONT_DOOR_CLOSED;
      openPath = FRONT_DOOR_OPEN;
      sprite->Open(FRONT_DOOR_CLOSED);
    }
    collider->SetScale({1, 2});
    collider->SetOffset({0, 1});
  }

  blocker_go->AddComponent(blocker);
  blockerGo = state.AddObject(blocker_go);

  GameObject* openGo = new GameObject();
  ActionMessage* openMsg =
      new ActionMessage(*openGo, position, "assets/img/open_msg.png");
  openGo->AddComponent(openMsg);

  openDoorGo = state.AddObject(openGo);

  GameObject* closeGo = new GameObject();
  ActionMessage* closeMsg =
      new ActionMessage(*closeGo, position, "assets/img/close_msg.png");
  closeGo->AddComponent(closeMsg);

  closeDoorGo = state.AddObject(closeGo);

  this->blocker = std::dynamic_pointer_cast<Blocker>(
      blockerGo->GetComponent(Types::BlockerType));

  GameObject* pcGo = new GameObject(associated.priority);
  pcGo->box = associated.box;
  PriorityChanger* priChanger = new PriorityChanger(*pcGo, associated, false);
  pcGo->AddComponent(priChanger);
  state.AddObject(pcGo);

  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Door::~Door() {
  closeDoorGo->RequestDelete();
  openDoorGo->RequestDelete();
  blockerGo->RequestDelete();
}

void Door::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }
}

void Door::Start() {}

void Door::Update(float dt) {}

void Door::Render() {
  if (colliding) {
    if (open)
      ShowCloseDialog();
    else
      ShowOpenDialog();
  } else {
    HideOpenDialog();
    HideCloseDialog();
  }
  colliding = false;
}

void Door::ShowOpenDialog() { openDoorGo->EnableRender(); }

void Door::HideOpenDialog() { openDoorGo->DisableRender(); }

void Door::ShowCloseDialog() { closeDoorGo->EnableRender(); }

void Door::HideCloseDialog() { closeDoorGo->DisableRender(); }

void Door::Open() {
  blocker->Unblock();
  open = true;
  HideOpenDialog();
  auto spriteCpt = associated.GetComponent(SpriteType);

  if (!spriteCpt) {
    throw std::runtime_error("Nao tem sprite na door!");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  sprite->Open(openPath);
}

void Door::Close() {
  blocker->Block();
  open = !blocker->IsBlocking();
  if (!open) {
    HideCloseDialog();
    auto spriteCpt = associated.GetComponent(SpriteType);

    if (!spriteCpt) {
      throw std::runtime_error("Nao tem sprite na door!");
    }

    auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
    sprite->Open(closePath);
  }
}

bool Door::IsOpen() { return open; }

bool Door::Is(Types type) const { return type == this->Type; }
