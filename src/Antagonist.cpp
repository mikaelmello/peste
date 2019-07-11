#include "Antagonist.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "AttackFSM.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "IdleFSM.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"
#include "Player.hpp"
#include "PriorityChanger.hpp"
#include "Sound.hpp"
#include "Sprite.hpp"

std::stack<std::pair<unsigned, std::vector<Vec2>>> Antagonist::paths;

Antagonist::Antagonist(GameObject& associated, std::vector<Vec2> path)
    : Component(associated), position(0, 0), stored_state(nullptr), path(path) {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();

  if (path.empty()) {
    throw std::invalid_argument("antagonist without path");
  }

  position = path[0];

  Sound* sound = new Sound(associated, BREATHING_ANTAGONIST_SOUND);
  Sprite* sprite = new Sprite(associated, DOWN_IDLE_SPRITE_ANTAGONIST, 4,
                              CALC_CONST / PATROL_SPEED);
  Collider* collider =
      new Collider(associated, {0.6, 0.15}, {0, sprite->GetHeight() * 0.45f});

  GameObject* pc_go = new GameObject();
  pc_go->box = associated.box;
  PriorityChanger* priority_changer =
      new PriorityChanger(*pc_go, associated, PCType::PCMonster);
  pc_go->AddComponent(priority_changer);
  state.AddObject(pc_go);

  int tileDim = 8;
  // a position está no meio horizontal e no fim vertical do sprite
  // para renderizar, colocamos o xy da box de acordo
  // posição * dimensão do tile - (comprimento da sprite / 2), pois o x fica no
  // meio da sprite
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  // posição * dimensão do tile - altura da sprite, pois o y fica la embaixo
  associated.box.y = position.y * tileDim - sprite->GetHeight();

  associated.AddComponent(sound);
  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Antagonist::~Antagonist() { delete stored_state; }

void Antagonist::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto player_cpt = other->GetComponent(Types::PlayerType);
  if (player_cpt && (last_action != Helpers::Action::ATTACKING)) {
    Push(new AttackFSM(associated));
  }
}

void Antagonist::Start() {
  last_action = Helpers::Action::IDLE;
  last_direction = Helpers::Direction::NONE;

  stored_state = new PatrolFSM(associated, path);
  state_stack.emplace(stored_state);
  stored_state->OnStateEnter();
  stored_state = nullptr;

  GameObject* pc_go = new GameObject(associated.priority);
  pc_go->box = associated.box;
  PriorityChanger* priority_changer = new PriorityChanger(*pc_go, associated);
  pc_go->AddComponent(priority_changer);
  priorityChanger_go = Game::GetInstance().GetCurrentState().AddObject(pc_go);
}

void Antagonist::Update(float dt) {
  previous_position = position;

  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("Sprite nao encontrado no Antagonista");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  if (!state_stack.empty()) {
    auto& top_state = state_stack.top();
    if (top_state->PopRequested()) {
      top_state->OnStateExit();
      state_stack.pop();
    }

    if (stored_state != nullptr) {
      state_stack.emplace(stored_state);
      stored_state->OnStateEnter();
      stored_state = nullptr;
    }

    if (state_stack.empty()) {
      state_stack.emplace(new PatrolFSM(associated, path));
      auto& state = state_stack.top();
      state->OnStateEnter();
    }

    auto& state = state_stack.top();
    state->Update(dt);
  }

  int tileDim = 8;
  // a position está no meio horizontal e no fim vertical do sprite
  // para renderizar, colocamos o xy da box de acordo
  // posição * dimensão do tile - (comprimento da sprite / 2), pois o x fica no
  // meio da sprite
  associated.box.w = sprite->GetWidth();
  associated.box.h = sprite->GetHeight();
  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  // posição * dimensão do tile - altura da sprite, pois o y fica la embaixo
  associated.box.y = position.y * tileDim - sprite->GetHeight();

  auto pcCpt = priorityChanger_go->GetComponent(PriorityChangerType);
  if (!pcCpt) {
    throw std::runtime_error("Sem PC CPT");
  }
  auto pc = std::dynamic_pointer_cast<PriorityChanger>(pcCpt);
  pc->SetRect(dt, associated.box);
}

void Antagonist::Render() {}

void Antagonist::Push(IFSM* s) {
  if (stored_state != nullptr) {
    delete stored_state;
  }
  stored_state = s;
}

bool Antagonist::NearTarget() {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  Pathfinder::Astar pf(associated, tilemap);

  if (GameData::MonsterFloor() != GameData::PlayerFloor()) {
    return false;
  }

  auto player = GameData::PlayerGameObject->GetComponent(PlayerType);
  if (!player) {
    throw std::runtime_error("Player game object without Player component");
  }
  auto playerCp = std::dynamic_pointer_cast<Player>(player);

  if (position == playerCp->position) {
    return true;
  }

  IFSM::Walkable w = IFSM::GetWalkable(associated, *GameData::PlayerGameObject);

  try {
    if (w.can_walk) {
      auto path = pf.Run(position, w.walkable);
      return path.size() != 0 && !GameData::player_is_hidden;
    }
    return false;

  } catch (const std::exception& ex) {
    printf("%s\n", ex.what());
    return false;
  }

  /*double dist = position.Distance(playerCp->position);
return dist <= distance_check && !GameData::player_is_hidden;*/
}

void Antagonist::AssetsManager(Helpers::Action action) {
  bool action_change = action != last_action;

  switch (action) {
    case Helpers::Action::IDLE:
      IdleAssetsManager(0.125, action_change);
      break;
    case Helpers::Action::MOVING:
      MoveAssetsManager(WALKING_WALK_SET, 0.18, action_change);
      break;
    case Helpers::Action::SUSPECTING:
      MoveAssetsManager(SUSPECTING_WALK_SET, 0.12, action_change);
      break;
    case Helpers::Action::CHASING:
      MoveAssetsManager(CHASING_WALK_SET, 0.06, action_change);
      break;
    case Helpers::Action::ATTACKING:
      AttackAssetsManager(0.125);
      break;
    default:
      action = Helpers::Action::IDLE;
      IdleAssetsManager(0.125, true);
      break;
  }
  last_action = action;
}

void Antagonist::MoveAssetsManager(std::vector<std::string> set,
                                   float frame_time, bool ac) {
  if (set.size() != 8) {
    throw std::runtime_error("O set de MoveAssetsManager deve ter 8 imagens");
  }

  auto soundCpt = associated.GetComponent(SoundType);
  if (!soundCpt) {
    throw std::runtime_error("O gameobject do antagonista não tem sound");
  }

  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sprite");
  }

  if (ac) {
    auto sound = std::dynamic_pointer_cast<Sound>(soundCpt);
    sound->Stop();
    sound->Open(ANTAGONIST_WALKING_SOUND);
    sound->Play();
  }

  Vec2 delta = position - previous_position;

  bool up = delta.y < 0;
  bool down = delta.y > 0;
  bool left = delta.x < 0;
  bool right = delta.x > 0;

  auto direction = Helpers::combine_moves(up, down, left, right);

  if (last_direction == direction || direction == Helpers::Direction::NONE) {
    return;
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  switch (direction) {
    case Helpers::Direction::LEFT:
      sprite->Open(set[0]);
      sprite->SetFrameCount(8);
      break;
    case Helpers::Direction::RIGHT:
      sprite->Open(set[1]);
      sprite->SetFrameCount(8);
      break;
    case Helpers::Direction::DOWN:
      sprite->Open(set[2]);
      sprite->SetFrameCount(5);
      break;
    case Helpers::Direction::UP:
      sprite->Open(set[3]);
      sprite->SetFrameCount(5);
      break;
    case Helpers::Direction::DOWNLEFT:
      sprite->Open(set[4]);
      sprite->SetFrameCount(5);
      break;
    case Helpers::Direction::UPLEFT:
      sprite->Open(set[5]);
      sprite->SetFrameCount(5);
      break;
    case Helpers::Direction::DOWNRIGHT:
      sprite->Open(set[6]);
      sprite->SetFrameCount(5);
      break;
    case Helpers::Direction::UPRIGHT:
      sprite->Open(set[7]);
      sprite->SetFrameCount(5);
      break;
    default:
      sprite->Open(set[2]);
      sprite->SetFrameCount(5);
      break;
  }

  sprite->SetFrameTime(frame_time);
  last_direction = direction;
}

void Antagonist::IdleAssetsManager(float frame_time, bool action_change) {
  if (!action_change) return;

  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sprite");
  }

  auto soundCpt = associated.GetComponent(SoundType);
  if (!soundCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sound");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  auto sound = std::dynamic_pointer_cast<Sound>(soundCpt);

  sound->Stop();
  sound->Open(BREATHING_ANTAGONIST_SOUND);
  sound->Play();

  switch (last_direction) {
    case Helpers::Direction::RIGHT:
      sprite->Open(RIGHT_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    case Helpers::Direction::LEFT:
      sprite->Open(LEFT_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    case Helpers::Direction::UP:
      sprite->Open(UP_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    case Helpers::Direction::DOWN:
      sprite->Open(DOWN_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    case Helpers::Direction::UPRIGHT:
      sprite->Open(UPRIGHT_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    case Helpers::Direction::UPLEFT:;
      sprite->Open(UPLEFT_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    case Helpers::Direction::DOWNRIGHT:
      sprite->Open(DOWNRIGHT_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    case Helpers::Direction::DOWNLEFT:
      sprite->Open(DOWNLEFT_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
    default:
      sprite->Open(DOWN_IDLE_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(4);
      break;
  }
}

void Antagonist::AttackAssetsManager(float frame_time) {
  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sprite");
  }

  auto soundCpt = associated.GetComponent(SoundType);
  if (!soundCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sound");
  }

  auto sound = std::dynamic_pointer_cast<Sound>(soundCpt);
  if (last_action != Helpers::Action::ATTACKING || !sound->IsOpen()) {
    sound->Stop();
    sound->Open(ATTACK_ROAR_ANTAGONIST);
    sound->Play();
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  switch (last_direction) {
    case Helpers::Direction::RIGHT:
      sprite->Open(RIGHT_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    case Helpers::Direction::LEFT:
      sprite->Open(LEFT_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    case Helpers::Direction::UP:
      sprite->Open(UP_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    case Helpers::Direction::DOWN:
      sprite->Open(DOWN_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    case Helpers::Direction::UPRIGHT:
      sprite->Open(UPRIGHT_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    case Helpers::Direction::UPLEFT:;
      sprite->Open(UPLEFT_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    case Helpers::Direction::DOWNRIGHT:
      sprite->Open(DOWNRIGHT_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    case Helpers::Direction::DOWNLEFT:
      sprite->Open(DOWNLEFT_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
    default:
      sprite->Open(DOWN_ATTACK_SPRITE_ANTAGONIST);
      sprite->SetFrameCount(3);
      break;
  }

  sprite->SetFrameTime(frame_time);
}

bool Antagonist::Is(Types type) const { return type == this->Type; }

void Antagonist::NewPatrolPath(std::vector<Vec2> path) {
  if (path.empty()) {
    throw std::invalid_argument(
        "Antagonist::NewPatrolPath - path must have size > 0");
  }

  while (!Antagonist::paths.empty()) {
    Antagonist::paths.pop();
  }

  while (!state_stack.empty()) {
    state_stack.pop();
  }

  this->path = path;
  position = path[0];
  state_stack.emplace(new PatrolFSM(associated, path));
}
