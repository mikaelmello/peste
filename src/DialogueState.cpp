#include "DialogueState.hpp"
#include <iostream>
#include "Camera.hpp"
#include "Dialogue.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

DialogueState::DialogueState(
    const std::vector<std::pair<std::string, std::string>>& script)
    : dialogueIndex(0), script(script) {
  Camera::Unfollow();
}

DialogueState::~DialogueState() {
  Camera::Follow(GameData::PlayerGameObject.get());
}

void DialogueState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  InputManager& im = InputManager::GetInstance();

  quitRequested |= im.QuitRequested();
  popRequested |= im.KeyPress(ESCAPE_KEY);

  if (im.KeyPress(X_KEY)) {
    dialogueIndex += 3;
  }

  if (dialogueIndex >= objects.size()) {
    popRequested = true;
  }

  UpdateArray(dt);
  //std::cout << objects.size() << std::endl;
}

void DialogueState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void DialogueState::Pause() {}

void DialogueState::Resume() {}

void DialogueState::LoadAssets() {
  int i = 1;
  for (auto p : script) {
    auto dialogue_go = std::make_shared<GameObject>(i);
    dialogue_go->box = Rect(43, 500, 1024, 183);
    Dialogue* dialogue = new Dialogue(*dialogue_go, p.first, p.second);
    dialogue_go->AddComponent(dialogue);
    objects.push_back(dialogue_go);
    i += 3;
  }
}

void DialogueState::Render() {
  if (dialogueIndex < objects.size()) {
    objects[dialogueIndex]->Render();
    objects[dialogueIndex + 1]->Render();
    objects[dialogueIndex + 2]->Render();
  }
}
