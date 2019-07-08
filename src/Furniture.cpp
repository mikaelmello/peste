#include "Furniture.hpp"
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Sprite.hpp"

#define HIDE_MSG "assets/img/x.png"
#define LOOK_MSG "assets/img/x.png"

Furniture::Furniture(GameObject& associated, const std::string& file,
                     Vec2 position, Interaction interaction)
    : Component(associated), interact(false), colliding(false) {
  Collider* collider = new Collider(associated, {1.5, 1.5}, {0.75, 0.75});
  Sprite* sprite = new Sprite(associated, file);
  associated.AddComponent(collider);
  associated.AddComponent(sprite);

  State& state = Game::GetInstance().GetCurrentState();

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  if (interaction != Interaction::NONE) {
    interact = true;
    GameObject* interactmsg_go = new GameObject();
    ActionMessage* interactMsg;
    if (interaction == Interaction::HIDE) {
      interactMsg = new ActionMessage(*interactmsg_go, position, HIDE_MSG);
    } else if (interaction == Interaction::LOOK) {
      interactMsg = new ActionMessage(*interactmsg_go, position, LOOK_MSG);
    }
    interactmsg_go->AddComponent(interactMsg);
    interactMsgGo = state.AddObject(interactmsg_go);
  }

  GameObject* blocker_go = new GameObject(associated.priority);
  blocker_go->box = associated.box;
  Blocker* blocker = new Blocker(*blocker_go);
  blocker_go->AddComponent(blocker);
  blockerGo = state.AddObject(blocker_go);
}

Furniture::~Furniture() {
  blockerGo->RequestDelete();
  if (interact) {
    interactMsgGo->RequestDelete();
  }
}

void Furniture::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto playerComponent = other->GetComponent(PlayerType);
  if (playerComponent) {
    colliding = true;
  }
}

void Furniture::Start() {}

void Furniture::Update(float dt) {}

void Furniture::Render() {
  // if (colliding && interact) {
  //   ShowInteractionDialog();
  // } else {
  //   HideInteractionDialog();
  // }
  // colliding = false;
}

void Furniture::ShowInteractionDialog() { interactMsgGo->EnableRender(); }

void Furniture::HideInteractionDialog() { interactMsgGo->DisableRender(); }

bool Furniture::Is(Types type) const { return type == this->Type; }
