#include "Terry.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Dialog.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Lore.hpp"
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

  State& state = Game::GetInstance().GetCurrentState();

  associated.box.x = position.x * 8;
  associated.box.y = position.y * 8;
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();

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
      new ActionMessage(*talkGo, position, "assets/img/talk.png");
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

    InputManager& input = InputManager::GetInstance();
    if (input.KeyPress(X_KEY)) {
      Talk();
    }
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

void Terry::Talk() {
  std::vector<std::pair<std::string, std::string>> scripts[] = {
      {
          {"Hope", "Você vive aqui há muito tempo?"},
          {"Terry", "..."},
          {"Hope", "Sua casa é bonita. Bem grande..."},
          {"Terry", "... é, muito."},
          {"Hope", "Tem um monte de brinquedo aqui, vamos lá!"},
      },
      {
          {"Hope", "Você tem alguma brincadeira favorita?"},
          {"Terry", "... sim."},
          {"Hope", "Qual?"},
      },
      {
          {"Hope", "Tá tudo bem?"},
      },
  };
  Lore::NicePerson = true;
  GameData::InitDialog(scripts[rand() % 3]);
}

bool Terry::Is(Types type) const { return type == this->Type; }
