#include "EndingtwoState.hpp"

EndingtwoState::EndingtwoState(){}

EndingtwoState::~EndingtwoState() {}

void EndingtwoState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  UpdateArray(dt);
}

void EndingtwoState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void EndingtwoState::Pause() {}

void EndingtwoState::Resume() {}

void EndingtwoState::LoadAssets() {}

void EndingtwoState::Render() { RenderArray(); }
