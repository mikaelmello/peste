#include "TutorialState.hpp"
#include "InputManager.hpp"

TutorialState::TutorialState() : im(InputManager::GetInstance()) {}

TutorialState::~TutorialState() {}

void TutorialState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  popRequested |= im.KeyPress(ESCAPE_KEY);
  quitRequested |= im.QuitRequested();

  UpdateArray(dt);
}

void TutorialState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void TutorialState::Pause() {}

void TutorialState::Resume() {}

void TutorialState::LoadAssets() {
  GameObject* background_tutorial = new GameObject();
  Sprite* background_sprite =
      new Sprite(*background_tutorial, TUTORIAL_BACKGROUND_SPRITE);
  background_tutorial->AddComponent(background_sprite);
  objects.emplace_back(background_tutorial);
}

void TutorialState::Render() { RenderArray(); }
