#include "AttackFSM.hpp"

#include "Antagonist.hpp"
#include "Sprite.hpp"
AttackFSM::AttackFSM(GameObject& object) : IFSM(object) { OnStateEnter(); }

AttackFSM::~AttackFSM() {}

void AttackFSM::OnStateEnter() {
  auto ant_cpt = object.GetComponent(Types::AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("objeto nao possui cpt antagonista em AttackFSM");
  }

  auto ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);
  ant->AssetsManager(Helpers::Action::ATTACKING);
}

void AttackFSM::OnStateExecution() {
  auto sprite_cpt = object.GetComponent(Types::SpriteType);
  if (!sprite_cpt) {
    throw std::runtime_error("objeto nao possui cpt sprite em AttackFSM");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(sprite_cpt);
}

void AttackFSM::OnStateExit() {}

void AttackFSM::Update(float dt) {
  OnStateExecution();
  timer.Update(dt);
}