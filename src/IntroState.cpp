#include "IntroState.hpp"

IntroState::IntroState(){}

IntroState::~IntroState() {}

void IntroState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  UpdateArray(dt);
}

void IntroState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void IntroState::Pause() {}

void IntroState::Resume() {}

void IntroState::LoadAssets() {}

void IntroState::Render() { RenderArray(); }
