#include "SleepState.hpp"
#include "GameData.hpp"
#include "Lore.hpp"

SleepState::SleepState() : im(InputManager::GetInstance()) {}

SleepState::~SleepState() {}

void SleepState::Update(float dt) {
  quitRequested |= im.QuitRequested();

  popRequested = pop_timer.Get() >= 2;
  pop_timer.Update(dt);

  sprite_go->Render();

  if (popRequested) {
    auto sound_cpt = sound_go->GetComponent(SoundType);
    if (!sound_cpt) {
      throw std::runtime_error("sem sound SleepState::Update");
    }
    auto sound = std::dynamic_pointer_cast<Sound>(sound_cpt);
    sound->Play(2);
    return;
  }
}

void SleepState::Start() {
  Lore::Slept++;
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

  sprite_go = std::make_shared<GameObject>();
  auto sprite_cpt = new Sprite(*sprite_go, SLEEPING_BG);
  sprite_cpt->SetScaleX(1.1, 1.1);
  sprite_go->AddComponent(sprite_cpt);
  sprite_go->box.x = 0;
  sprite_go->box.y = 0;
  objects.push_back(sprite_go);
}

void SleepState::Render() { RenderArray(); }
