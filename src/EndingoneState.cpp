#include "EndingoneState.hpp"

EndingoneState::EndingoneState(){}

EndingoneState::~EndingoneState() {}

void EndingoneState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  UpdateArray(dt);
}

void EndingoneState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void EndingoneState::Pause() {}

void EndingoneState::Resume() {}

void EndingoneState::LoadAssets() {}

void EndingoneState::Render() { RenderArray(); }
