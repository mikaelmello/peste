#include "SleepState.hpp"
#include "GameData.hpp"
#include "Lore.hpp"

SleepState::SleepState() : im(InputManager::GetInstance()) { Lore::Slept++; }

SleepState::~SleepState() {}

void SleepState::Update(float dt) {
  quitRequested |= im.QuitRequested();

  popRequested = pop_timer.Get() >= 2;
  pop_timer.Update(dt);

  if (popRequested) {
    auto sound_cpt = sound_go->GetComponent(SoundType);
    if (!sound_cpt) {
      throw std::runtime_error("sem sound SleepState::Update");
    }
    auto sound = std::dynamic_pointer_cast<Sound>(sound_cpt);
    sound->Play();
    return;
  }
}

void SleepState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void SleepState::Pause() {}

void SleepState::Resume() {}

void SleepState::LoadAssets() {
  sound_go = std::make_shared<GameObject>();
  auto sound_cpt = new Sound(*sound_go, WRECKING_SOUND);
  sound_go->AddComponent(sound_cpt);
  objects.push_back(sound_go);
}

void SleepState::Render() { RenderArray(); }
