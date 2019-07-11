#include "Furniture.hpp"
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "PriorityChanger.hpp"
#include "Sound.hpp"
#include "Sprite.hpp"

#define HIDE_MSG "assets/img/hide.png"
#define LOOK_MSG "assets/img/look.png"
#define OUT_MSG "assets/img/goOut.png"
#define SLEEP_MSG "assets/img/sleep.png"

Furniture::Furniture(GameObject& associated, const std::string& file,
                     Vec2 position, Helpers::Interaction interaction,
                     bool fullblock, std::vector<std::string> script,
                     bool special)
    : Component(associated),
      interact(false),
      colliding(false),
      interaction(interaction),
      script(script),
      special(special) {
  Sprite* sprite = new Sprite(associated, file);
  associated.AddComponent(sprite);

  Sound* sound = new Sound(associated);
  associated.AddComponent(sound);

  State& state = Game::GetInstance().GetCurrentState();

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

  if (interaction != Helpers::Interaction::NOTHING) {
    Collider* collider = new Collider(associated, {1, 1.5},
                                      {0, (float)(sprite->GetHeight() * 0.25)});
    associated.AddComponent(collider);

    interact = true;

    GameObject* interactmsg_go = new GameObject();
    ActionMessage* interactMsg;

    if (interaction == Helpers::Interaction::HIDE) {
      interactMsg = new ActionMessage(*interactmsg_go, position, HIDE_MSG);
      collider->SetScale({1.5, 1.5});
    } else if (interaction == Helpers::Interaction::LOOK ||
               interaction == Helpers::Interaction::PLAY) {
      interactMsg = new ActionMessage(*interactmsg_go, position, LOOK_MSG);
      // collider->SetScale({1, 1})
    } else if (interaction == Helpers::Interaction::SLEEP) {
      interactMsg = new ActionMessage(*interactmsg_go, position, SLEEP_MSG);
      collider->SetScale({1.5, 1.5});
    }
    interactmsg_go->AddComponent(interactMsg);
    interactMsgGo = state.AddObject(interactmsg_go);
  }

  GameObject* blocker_go = new GameObject(associated.priority);
  blocker_go->box = associated.box;
  Blocker* blocker;
  if (fullblock) {
    blocker = new Blocker(*blocker_go);
  } else {
    blocker =
        new Blocker(*blocker_go, {1, 0.4}, {0, sprite->GetHeight() * 0.30f});

    GameObject* pcGo = new GameObject(associated.priority);
    pcGo->box = associated.box;
    PriorityChanger* priChanger = new PriorityChanger(*pcGo, associated);
    pcGo->AddComponent(priChanger);
    state.AddObject(pcGo);
  }

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

void Furniture::Update(float dt) {
  if (interaction == Helpers::Interaction::HIDE) {
    auto cpt = interactMsgGo->GetComponent(ActionMessageType);
    auto action_message = std::dynamic_pointer_cast<ActionMessage>(cpt);
    if (GameData::player_is_hidden) {
      action_message->UpdateSprite(OUT_MSG);
    } else {
      action_message->UpdateSprite(HIDE_MSG);
    }
  }
}

void Furniture::Render() {
  if (interact) {
    if (colliding && interact) {
      ShowInteractionDialog();
    } else {
      HideInteractionDialog();
    }
  }
  colliding = false;
}

void Furniture::ShowInteractionDialog() { interactMsgGo->EnableRender(); }

void Furniture::HideInteractionDialog() { interactMsgGo->DisableRender(); }

Helpers::Interaction Furniture::GetInteraction() { return interaction; }

void Furniture::RemoveInteraction() {
  interaction = Helpers::Interaction::NOTHING;
  interactMsgGo->RequestDelete();
  interact = false;
}

void Furniture::Look() {
  SCRIPT_TYPE s;
  for (auto aux : script) {
    s.push_back({"HOPE", aux});
  }
  if (special && GameData::got_key1) {
    s.pop_back();
  }

  GameData::InitDialog(s);
}

bool Furniture::Is(Types type) const { return type == this->Type; }
