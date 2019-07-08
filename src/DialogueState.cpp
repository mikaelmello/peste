#include "DialogueState.hpp"
#include "Camera.hpp"
#include "Dialogue.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

DialogueState::DialogueState() : dialogueIndex(0) { Camera::Unfollow(); }

DialogueState::~DialogueState() { Camera::Follow(GameData::PlayerGameObject); }

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

void DialogueState::LoadAssets() {
  Rect dialogueBox(43, 500, 1024, 183);

  GameObject* dialogue_go1 = new GameObject(1);
  dialogue_go1->box = dialogueBox;
  Dialogue* dialogue1 = new Dialogue(
      *dialogue_go1, "HOPE",
      "Hope esta falando besteira para o Terry e Lorem Ipsum is simply dummy "
      "text of the printing and typesetting industry. Lorem Ipsum has been the "
      "industry's standard dummy text ever since the 1500s, when an unknown "
      "printer took a galley of type and scrambled it to make a type specimen "
      "book.");
  dialogue_go1->AddComponent(dialogue1);
  objects.emplace_back(dialogue_go1);

  GameObject* dialogue_go2 = new GameObject(3);
  dialogue_go2->box = dialogueBox;
  Dialogue* dialogue2 =
      new Dialogue(*dialogue_go2, "TERRY", "Terry esta pistolito com o Hope.");
  dialogue_go2->AddComponent(dialogue2);
  objects.emplace_back(dialogue_go2);

  SortObjects();
}

void DialogueState::Render() {
  if (dialogueIndex + 2 < objects.size()) {
    objects[dialogueIndex]->Render();
    objects[dialogueIndex + 1]->Render();
    objects[dialogueIndex + 2]->Render();
  }
}
