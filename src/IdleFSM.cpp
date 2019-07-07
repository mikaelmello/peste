#include "IdleFSM.hpp"
#include "Antagonist.hpp"
#include "Types.hpp"

IdleFSM::IdleFSM(GameObject& object) : IFSM(object) {
  auto ant_cpt = object.GetComponent(Types::AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("sem antagonista em IdleFSM::IdleFSM");
  }
  ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);
}

IdleFSM::~IdleFSM() {}

void IdleFSM::OnStateEnter() {
  ant.lock()->AssetsManager(Helpers::Action::IDLE);
}

void IdleFSM::OnStateExecution() {}

void IdleFSM::OnStateExit() {}

void IdleFSM::Update(float dt) {
  pop_requested = timer.Get() >= IDLE_TIME;
  timer.Update(dt);
}
