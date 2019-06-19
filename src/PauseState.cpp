#include "PauseState.hpp"

PauseState::PauseState(){}

PauseState::~PauseState() {}

void PauseState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  UpdateArray(dt);
}

void PauseState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void PauseState::Pause() {}

void PauseState::Resume() {}

void PauseState::LoadAssets() {}

void PauseState::Render() { RenderArray(); }
