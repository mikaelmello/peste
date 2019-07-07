#include "DialogueState.hpp"

DialogueState::DialogueState(){}

DialogueState::~DialogueState() {}

void DialogueState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
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

void DialogueState::Render() { RenderArray(); }
