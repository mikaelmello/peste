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
  execution_time = sprite->GetFrameCount() * sprite->GetFrameTime();
}

void AttackFSM::OnStateExecution() {
  auto ant_col = object.GetComponent(ColliderType);
  if (!ant_col) {
    throw std::runtime_error("ant sem collider em AttackFSM::OnStateExecution");
  }

  auto player_col = GameData::PlayerGameObject->GetComponent(ColliderType);
  if (!player_col) {
    throw std::runtime_error("player sem col em AttackFSM::OnStateExecution");
  }

  Rect a_box = std::dynamic_pointer_cast<Collider>(ant_col)->box;
  Rect p_box = std::dynamic_pointer_cast<Collider>(player_col)->box;

  bool collides = Collision::IsColliding(a_box, p_box);

  if (collides) {
    GameData::player_was_hit = true;
  }
}

void AttackFSM::OnStateExit() {}

void AttackFSM::Update(float dt) {
  auto ant_col = object.GetComponent(ColliderType);
  if (!ant_col) {
    throw std::runtime_error("ant sem collider em AttackFSM::Update");
  }

  auto player_col = GameData::PlayerGameObject->GetComponent(ColliderType);
  if (!player_col) {
    throw std::runtime_error("player sem col em AttackFSM::Update");
  }

  auto ant_cpt = object.GetComponent(Types::AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("object sem antagonist em AttackFSM::Update");
  }

  auto ant = std::dynamic_pointer_cast<Antagonist>(ant_cpt);
  ant->AssetsManager(Helpers::Action::ATTACKING);

  if (timer.Get() >= execution_time) {
    OnStateExecution();
  }

  Rect a_box = std::dynamic_pointer_cast<Collider>(ant_col)->box;
  Rect p_box = std::dynamic_pointer_cast<Collider>(player_col)->box;
  if (!Collision::IsColliding(a_box, p_box)) {
    pop_requested = true;
  }

  timer.Update(dt);
}