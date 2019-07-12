#include "CaughtState.hpp"
#include "Lore.hpp"

CaughtState::CaughtState() : im(InputManager::GetInstance()) {}

CaughtState::~CaughtState() {}

void CaughtState::Update(float dt) {
  quitRequested |= im.QuitRequested();

  popRequested = pop_timer.Get() >= 2;
  pop_timer.Update(dt);

  if (popRequested || quitRequested) {
    return;
  }

  UpdateArray(dt);
}

void CaughtState::Start() {
  LoadAssets();
  StartArray();
  started = true;
  Lore::Caught = true;
}

void CaughtState::Pause() {}

void CaughtState::Resume() {}

void CaughtState::LoadAssets() {}

void CaughtState::Render() { RenderArray(); }
