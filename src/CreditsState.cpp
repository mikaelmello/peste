#include "CreditsState.hpp"

CreditsState::CreditsState(){}

CreditsState::~CreditsState() {}

void CreditsState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  UpdateArray(dt);
}

void CreditsState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void CreditsState::Pause() {}

void CreditsState::Resume() {}

void CreditsState::LoadAssets() {}

void CreditsState::Render() { RenderArray(); }
