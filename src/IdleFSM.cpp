#include "IdleFSM.hpp"

#include "Sprite.hpp"

IdleFSM::IdleFSM(GameObject& antagonist) : IFSM(antagonist) { OnStateEnter(); }

IdleFSM::~IdleFSM() {}

void IdleFSM::OnStateEnter() {
  auto sprite = std::dynamic_pointer_cast<Sprite>(
      antagonist.GetComponent(GameData::Sprite).lock());
  sprite->Open(IDLE_SPRITE_ANTAGONIST);

  timer.Restart();
}

void IdleFSM::OnStateExecution() {
  if (timer.Get() > IDLE_TIME) {
    pop_requested = true;
  }
}

void IdleFSM::OnStateExit() {}

void IdleFSM::Update(float dt) {
  OnStateExecution();
  timer.Update(dt);
}