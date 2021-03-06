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

#define FRONT_DOOR_CLOSED "assets/img/doors/door_front_closed.png"
#define FRONT_DOOR_OPEN "assets/img/doors/door_front_open.png"
#define DOUBLE_DOOR_CLOSED "assets/img/doors/doubleDoor_front_closed.png"
#define DOUBLE_DOOR_OPEN "assets/img/doors/doubleDoor_front_open.png"
#define LAB_DOOR_OPEN "assets/img/doors/labdoorOpen.png"
#define LAB_DOOR_CLOSED "assets/img/doors/labdoor.png"
#define MAIN_DOOR "assets/img/doors/maindoor.png"

Door::Door(GameObject& associated, Helpers::DoorStyle style, Vec2 position,
           bool open, Helpers::KeyType key)
    : Component(associated),
      position(position),
      colliding(false),
      open(open),
      key(key) {
  Collider* collider = new Collider(associated);
  Sprite* sprite = new Sprite(associated);

  if (style == Helpers::DoorStyle::DOUBLE) {
    closePath = DOUBLE_DOOR_CLOSED;
    openPath = DOUBLE_DOOR_OPEN;
    sprite->Open(DOUBLE_DOOR_CLOSED);
  } else if (style == Helpers::DoorStyle::SINGLE) {
    closePath = FRONT_DOOR_CLOSED;
    openPath = FRONT_DOOR_OPEN;
    sprite->Open(FRONT_DOOR_CLOSED);
  } else if (style == Helpers::DoorStyle::LAB) {
    closePath = LAB_DOOR_CLOSED;
    openPath = LAB_DOOR_OPEN;
    sprite->Open(LAB_DOOR_CLOSED);
  } else if (style == Helpers::DoorStyle::MAIN) {
    sprite->Open(MAIN_DOOR);
  }
  collider->SetScale({1, 1.5});
  collider->SetOffset({0, 0.75});

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  GameObject* blocker_go = new GameObject(associated.priority);
  blocker_go->box = associated.box;
  Blocker* blocker =
      new Blocker(*blocker_go, {1, 0.4}, {0, sprite->GetHeight() * 0.30f});

  GameObject* openGo = new GameObject();
  ActionMessage* openMsg =
      new ActionMessage(*openGo, position, "assets/img/open_msg.png");
  openGo->AddComponent(openMsg);

  State& state = Game::GetInstance().GetCurrentState();
  openDoorGo = state.AddObject(openGo);

  GameObject* closeGo = new GameObject();
  ActionMessage* closeMsg =
      new ActionMessage(*closeGo, position, "assets/img/close_msg.png");
  closeGo->AddComponent(closeMsg);

  closeDoorGo = state.AddObject(closeGo);

  blocker_go->AddComponent(blocker);
  auto blocker_ptr = state.AddObject(blocker_go);

  this->blocker = std::dynamic_pointer_cast<Blocker>(
      blocker_ptr->GetComponent(Types::BlockerType));

  GameObject* pcGo = new GameObject(associated.priority);
  pcGo->box = associated.box;
  PriorityChanger* priChanger = new PriorityChanger(*pcGo, associated);
  pcGo->AddComponent(priChanger);
  state.AddObject(pcGo);

  associated.AddComponent(collider);
  associated.AddComponent(sprite);

  if (open) Open();
}

Door::~Door() {}

void Door::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }

  auto antComponent = other->GetComponent(AntagonistType);
  if (antComponent && this->key == Helpers::KeyType::NOKEY) {
    Open();
  }
}

void Door::Start() {}

void Door::Update(float dt) {}

void Door::Render() {
  if (colliding) {
    if (open) {
      ShowCloseDialog();
    } else {
      ShowOpenDialog();
    }
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

Helpers::KeyType Door::GetKey() { return key; }

void Door::SetKey(Helpers::KeyType k) { key = k; }

bool Door::Is(Types type) const { return type == this->Type; }
