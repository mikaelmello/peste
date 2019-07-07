#include "AttackFSM.hpp"

#include <memory>
#include "Antagonist.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Player.hpp"
#include "Sprite.hpp"

AttackFSM::AttackFSM(GameObject& object) : IFSM(object) {
  auto ant_cpt = object.GetComponent(Types::AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("sem ant em AttackFSM::AttackFSM");
  }

  ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);
}

AttackFSM::~AttackFSM() {}

void AttackFSM::OnStateEnter() {
  auto sprite_cpt = object.GetComponent(Types::SpriteType);
  if (!sprite_cpt) {
    throw std::runtime_error("objeto nao possui cpt sprite em AttackFSM");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(sprite_cpt);
  execution_time = sprite->GetFrameCount() * sprite->GetFrameTime();

  ant.lock()->AssetsManager(Helpers::Action::ATTACKING);
}

void AttackFSM::OnStateExecution() {
  GameData::player_was_hit = IsColliding();
  pop_requested = true;
}

void AttackFSM::OnStateExit() {}

void AttackFSM::Update(float dt) {
  if (timer.Get() >= execution_time) {
    OnStateExecution();
  }

  pop_requested |= !IsColliding();
  timer.Update(dt);
}

bool AttackFSM::IsColliding() {
  auto a_col = object.GetComponent(ColliderType);
  if (!a_col) {
    throw std::runtime_error("ant sem collider em AttackFSM::IsColliding");
  }

  auto p_col = GameData::PlayerGameObject->GetComponent(ColliderType);
  if (!p_col) {
    throw std::runtime_error("player sem col em AttackFSM::IsColliding");
  }

  auto ant_col = std::dynamic_pointer_cast<Collider>(a_col);
  auto player_col = std::dynamic_pointer_cast<Collider>(p_col);

  Rect a_box = ant_col->box;
  Rect p_box = ant_col->box;

  return Collision::IsColliding(a_box, p_box);
}