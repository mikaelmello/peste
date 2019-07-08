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

  // State& state = Game::GetInstance().GetCurrentState();

  // GameObject* interlocutor_go = new GameObject(associated.priority + 1);
  associated.box.x = associated.box.x + 35;
  associated.box.y = associated.box.y + 20;
  Text* itor = new Text(associated, "assets/font/tox-typewriter.ttf", 25,
                        Text::BLENDED_WRAPPED, interlocutor, {0, 0, 0, 0},
                        associated.box.w - 26);
  associated.AddComponent(itor);
  // interlocutorGo = state.AddObject(interlocutor_go);

  // GameObject* text_go = new GameObject(associated.priority + 1);
  associated.box.x = associated.box.x + 25;
  associated.box.y = associated.box.y + 45;
  Text* txt = new Text(associated, "assets/font/tox-typewriter.ttf", 25,
                       Text::BLENDED_WRAPPED, text, {0, 0, 0, 0},
                       associated.box.w - 26);
  associated.AddComponent(txt);
  // textGo = state.AddObject(text_go);
  std::cout << "ok" << std::endl;
}

Dialogue::~Dialogue() {}

void Dialogue::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Dialogue::Start() {}

void Dialogue::Update(float dt) {}

void Dialogue::Render() {}

bool Dialogue::Is(Types type) const { return type == this->Type; }