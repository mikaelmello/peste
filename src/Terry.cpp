#include "Terry.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "ActionMessage.hpp"
#include "Blocker.hpp"
#include "Collider.hpp"
#include "Dialogue.hpp"
#include "DialogueState.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
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
  std::vector<std::pair<std::string, std::string>> scripts;
  std::pair<std::string, std::string> aux;
  aux.first = "HOPE";
  aux.second =
      "Hope esta falando besteira para o Terry e Lorem Ipsum is simply dummy "
      "text of the printing and typesetting industry. Lorem Ipsum has been "
      "the "
      "industry's standard dummy text ever since the 1500s, when an unknown "
      "printer took a galley of type and scrambled it to make a type "
      "specimen "
      "book.";
  scripts.push_back(aux);

  aux.first = "TERRY";
  aux.second = "Terry esta pistolito com o Hope.";
  scripts.push_back(aux);

  GameData::InitDialog(scripts);
}

bool Terry::Is(Types type) const { return type == this->Type; }
