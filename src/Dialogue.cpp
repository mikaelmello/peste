#include "Dialogue.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

Dialogue::Dialogue(GameObject& associated, const std::string& interlocutor,
                   const std::string& text)
    : Component(associated) {
  Sprite* sprite = new Sprite(associated, "assets/img/dialogue.png");
  associated.AddComponent(sprite);

  State& state = Game::GetInstance().GetCurrentState();

  GameObject* interlocutor_go = new GameObject(associated.priority + 1);
  interlocutor_go->box.x = associated.box.x + 35;
  interlocutor_go->box.y = associated.box.y + 20;
  Text* itor = new Text(*interlocutor_go, "assets/font/tox-typewriter.ttf", 25,
                        Text::BLENDED_WRAPPED, interlocutor, {0, 0, 0, 0},
                        associated.box.w - 26);
  interlocutor_go->AddComponent(itor);
  interlocutorGo = state.AddObject(interlocutor_go);

  GameObject* text_go = new GameObject(associated.priority + 1);
  text_go->box.x = associated.box.x + 25;
  text_go->box.y = associated.box.y + 45;
  Text* txt = new Text(*text_go, "assets/font/tox-typewriter.ttf", 25,
                       Text::BLENDED_WRAPPED, text, {0, 0, 0, 0},
                       associated.box.w - 26);
  text_go->AddComponent(txt);
  textGo = state.AddObject(text_go);
}

Dialogue::~Dialogue() {}

void Dialogue::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Dialogue::Start() {}

void Dialogue::Update(float dt) {}

void Dialogue::Render() {}

bool Dialogue::Is(Types type) const { return type == this->Type; }
