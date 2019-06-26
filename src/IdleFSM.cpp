#include "IdleFSM.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

IdleFSM::IdleFSM(GameObject& object) : IFSM(object) { OnStateEnter(); }

IdleFSM::~IdleFSM() {}

void IdleFSM::OnStateEnter() {
  auto spriteCpt = object.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("Nao tem sprite no OnStateEnter do IdleFSM");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
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
