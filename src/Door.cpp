#include "Door.hpp"
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Sprite.hpp"
#include "State.hpp"

#define LEFT_DOOR "assets/img/doors/left_door.png"
#define RIGHT_DOOR "assets/img/doors/left_door.png"
#define UP_DOOR "assets/img/doors/left_door.png"

Door::Door(GameObject& associated, Helpers::Direction direction, Vec2 position,
           bool open)
    : Component(associated), position(position), open(open) {
  Collider* collider = new Collider(associated);
  Sprite* sprite = new Sprite(associated);

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  if (direction == Helpers::Direction::RIGHT) {
    sprite->Open(LEFT_DOOR);
    collider->SetScale({2, 1.5});
    collider->SetOffset({1, 0.75});
  } else if (direction == Helpers::Direction::LEFT) {
    sprite->Open(RIGHT_DOOR);
    collider->SetScale({4, 1.5});
    collider->SetOffset({2, 0.75});
  } else if (direction == Helpers::Direction::UP) {
    sprite->Open(UP_DOOR);
    collider->SetScale({1, 0.3});
  }

  GameObject* openGo = new GameObject(associated.priority);
  ActionMessage* openMsg =
      new ActionMessage(*openGo, position, "assets/img/open_msg.png");
  openGo->AddComponent(openMsg);

  State& state = Game::GetInstance().GetCurrentState();
  openDoorGo = state.AddObject(openGo);

  GameObject* closeGo = new GameObject(associated.priority);
  ActionMessage* closeMsg =
      new ActionMessage(*closeGo, position, "assets/img/close_msg.png");
  closeGo->AddComponent(closeMsg);

  closeDoorGo = state.AddObject(closeGo);

  GameObject* blocker_go = new GameObject(associated.priority);
  blocker_go->box = associated.box;
  Blocker* blocker = new Blocker(*blocker_go);
  blocker_go->AddComponent(blocker);
  auto blocker_ptr = state.AddObject(blocker_go);

  this->blocker = std::dynamic_pointer_cast<Blocker>(
      blocker_ptr->GetComponent(Types::BlockerType));

  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Door::~Door() {}

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
}

void Door::Close() {
  blocker->Block();
  open = !blocker->IsBlocking();
  if (!open) HideCloseDialog();
}

bool Door::IsOpen() { return open; }

bool Door::Is(Types type) const { return type == this->Type; }
