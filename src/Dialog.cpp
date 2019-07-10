#include "Dialog.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Camera.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

Dialog::Dialog(GameObject& associated) : Component(associated) {
  State& state = Game::GetInstance().GetCurrentState();
  Sprite* sprite = new Sprite(associated, "assets/img/dialogue.png");
  associated.AddComponent(sprite);
  associated.box = Rect(43, 500, 1024, 183);

  GameObject* interlocutor_go = new GameObject(associated.priority + 1);
  Text* interlocutor =
      new Text(*interlocutor_go, "assets/font/tox-typewriter.ttf", 25,
               Text::BLENDED_WRAPPED, " ", {0, 0, 0, 0}, associated.box.w - 26);
  interlocutor_go->AddComponent(interlocutor);
  interlocutorGo = state.AddObject(interlocutor_go);

  GameObject* text_go = new GameObject(associated.priority + 1);
  Text* txt =
      new Text(*text_go, "assets/font/tox-typewriter.ttf", 25,
               Text::BLENDED_WRAPPED, " ", {0, 0, 0, 0}, associated.box.w - 26);
  text_go->AddComponent(txt);
  textGo = state.AddObject(text_go);

  interlocutorGo->DisableRender();
  textGo->DisableRender();
  associated.DisableRender();
}

Dialog::~Dialog() {
  interlocutorGo->RequestDelete();
  textGo->RequestDelete();
}

void Dialog::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Dialog::Start() {}

void Dialog::Update(float dt) {
  if (script.size() == 0) return;

  InputManager& im = InputManager::GetInstance();

  if (im.KeyPress(SPACE_BAR_KEY)) {
    dialogIndex++;
    UpdateTexts();
  }

  if (dialogIndex >= script.size()) {
    Clear();
  }
}

void Dialog::Clear() {
  dialogIndex = 0;
  script.clear();

  interlocutorGo->DisableRender();
  textGo->DisableRender();
  associated.DisableRender();
}

void Dialog::Render() {}

void Dialog::UpdateTexts() {
  if (dialogIndex >= script.size()) {
    return;
  }

  auto cameraPos = Camera::pos;
  associated.box = Rect(43, 500, 1024, 183) + cameraPos;
  interlocutorGo->box.x = associated.box.x + 35;
  interlocutorGo->box.y = associated.box.y + 20;
  textGo->box.x = associated.box.x + 25;
  textGo->box.y = associated.box.y + 45;

  auto interlocutorCpt = interlocutorGo->GetComponent(TextType);
  auto textCpt = textGo->GetComponent(TextType);

  if (!interlocutorCpt || !textCpt) {
    throw std::runtime_error("Texts sem texts");
  }

  auto interlocutor = std::dynamic_pointer_cast<Text>(interlocutorCpt);
  auto text = std::dynamic_pointer_cast<Text>(textCpt);

  interlocutor->SetText(script[dialogIndex].first);
  text->SetText(script[dialogIndex].second);
}

bool Dialog::Is(Types type) const { return type == this->Type; }

void Dialog::SetScript(SCRIPT_TYPE script) {
  if (this->script.size() > 0) {
    throw std::runtime_error(
        "Mano para de colocar script quando ja tem um dialog!!");
  }

  if (script.size() == 0) {
    throw std::runtime_error("Mano para de colocar script vazio!!");
  }

  this->dialogIndex = 0;
  this->script = script;

  UpdateTexts();

  interlocutorGo->EnableRender();
  textGo->EnableRender();
  associated.EnableRender();
}
