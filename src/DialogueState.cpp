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
    : dialogueIndex(0) {
  int i = 1;
  for (auto p : script) {
    GameObject* dialogue_go = new GameObject(i);
    dialogue_go->box = Rect(43, 500, 1024, 183);
    Dialogue* dialogue = new Dialogue(*dialogue_go, p.first, p.second);
    dialogue_go->AddComponent(dialogue);
    objects.emplace_back(dialogue_go);
    i += 2;
  }
  std::cout << objects.size() << std::endl;
  SortObjects();

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
}

void DialogueState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void DialogueState::Pause() {}

void DialogueState::Resume() {}

void DialogueState::LoadAssets() {}

void DialogueState::Render() {
  if (dialogueIndex + 2 < objects.size()) {
    objects[dialogueIndex]->Render();
    objects[dialogueIndex + 1]->Render();
    objects[dialogueIndex + 2]->Render();
  }
}
