#include "AttackFSM.hpp"

#include <memory>
#include "Antagonist.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Player.hpp"
#include "Sprite.hpp"

AttackFSM::AttackFSM(GameObject& object) : IFSM(object) {}

AttackFSM::~AttackFSM() {}

void AttackFSM::OnStateEnter() {
  auto ant_cpt = object.GetComponent(Types::AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("objeto nao possui cpt antagonista em AttackFSM");
  }

  auto sprite_cpt = object.GetComponent(Types::SpriteType);
  if (!sprite_cpt) {
    throw std::runtime_error("objeto nao possui cpt sprite em AttackFSM");
  }

  auto ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);
  ant->AssetsManager(Helpers::Action::ATTACKING);

  auto sprite = std::dynamic_pointer_cast<Sprite>(sprite_cpt);
  execution_time = sprite->GetFrameCount() * sprite->GetFrameTime() + 5;
}

void AttackFSM::OnStateExecution() {}

void AttackFSM::OnStateExit() {}

void AttackFSM::Update(float dt) {
  if (timer.Get() >= execution_time) {
    OnStateExecution();
    pop_requested = true;
  }
  timer.Update(dt);
}