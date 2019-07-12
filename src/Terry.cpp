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

Terry::Terry(GameObject& associated, const std::string& file, Vec2 position)
    : Component(associated), position(position), colliding(false) {
  Sprite* sprite = new Sprite(associated, file);
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
    if (input.KeyPress(X_KEY) || input.JoyKeyPress(JOY_A_KEY)) {
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
  if (!GameData::transformed_monster_in_terry) {
    SCRIPT_TYPE scripts[] = {
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
  } else {
    SCRIPT_TYPE script = {
        {"Hope ", "Terry!"},
        {"Hope ", "Meu deus, é você mesmo?!"},
        {"Hope ", "Você está bem?!"},
        {"Terry ", "Você é... o médico?"},
        {"Hope ", "Sim! Meu nome é Hope, lembra?"},
        {"Terry ", "Você não foi... embora?"},
        {"Hope ", "Eu vim aqui para cuidar de você."},
        {"Terry ", "Você... é diferente de todos."},
        {"Terry ", "É diferente do... meu pai."},
        {"Hope ", "..."},
        {"Hope ", "Vem... vamos sair daqui."},
    };
    GameData::InitDialog(script);
  }
}

void Terry::SetSprite(const std::string& file) {
  auto sprite_cpt = associated.GetComponent(SpriteType);
  auto sprite = std::dynamic_pointer_cast<Sprite>(sprite_cpt);
  sprite->Open(file);
}

void Terry::SetAnimation(int frameCount, float frameTime) {
  auto sprite_cpt = associated.GetComponent(SpriteType);
  auto sprite = std::dynamic_pointer_cast<Sprite>(sprite_cpt);
  sprite->SetFrameCount(frameCount);
  sprite->SetFrameTime(frameTime);
}

bool Terry::Is(Types type) const { return type == this->Type; }
