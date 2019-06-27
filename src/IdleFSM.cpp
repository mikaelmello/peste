#include "IdleFSM.hpp"
#include "Antagonist.hpp"
#include "Types.hpp"

IdleFSM::IdleFSM(GameObject& object) : IFSM(object) { OnStateEnter(); }

IdleFSM::~IdleFSM() {}

void IdleFSM::OnStateEnter() {
  auto ant_cpt = object.GetComponent(Types::AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("Nao tem antagonist no OnStateEnter do IdleFSM");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);

  ant->AssetsManager(Helpers::Action::IDLE);
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
