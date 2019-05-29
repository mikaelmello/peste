#include "TutorialState.hpp"

TutorialState::TutorialState(){}

TutorialState::~TutorialState() {}

void TutorialState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  UpdateArray(dt);
}

void TutorialState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void TutorialState::Pause() {}

void TutorialState::Resume() {}

void TutorialState::LoadAssets() {}

void TutorialState::Render() { RenderArray(); }
