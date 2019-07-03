#include "CreditsState.hpp"

CreditsState::CreditsState() : im(InputManager::GetInstance()) {}

CreditsState::~CreditsState() {}

void CreditsState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  popRequested |= im.KeyPress(ESCAPE_KEY);
  quitRequested |= im.QuitRequested();

  UpdateArray(dt);
}

void CreditsState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void CreditsState::Pause() {}

void CreditsState::Resume() {}

void CreditsState::LoadAssets() {
  GameObject* background_go = new GameObject();
  Sprite* background_sprite =
      new Sprite(*background_go, CREDIT_BACKGROUND_SPRITE);
  background_go->AddComponent(background_sprite);
  objects.emplace_back(background_go);
}

void CreditsState::Render() { RenderArray(); }
