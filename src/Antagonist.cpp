#include "Antagonist.hpp"
#include <iostream>
#include <string>
#include "AttackFSM.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IdleFSM.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"
#include "Player.hpp"
#include "Sound.hpp"
#include "Sprite.hpp"

Antagonist::Antagonist(GameObject& associated, Vec2 position)
    : Component(associated), position(position), stored_state(nullptr) {
  Sound* sound = new Sound(associated, BREATHING_ANTAGONIST_SOUND);
  Sprite* sprite = new Sprite(associated, IDLE_SPRITE, 4, 0.125);
  Collider* collider = new Collider(associated, {0.6, 0.15},
                                    {0, (float)(sprite->GetHeight() * 0.45)});

  associated.AddComponent(sound);
  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Antagonist::~Antagonist() { delete stored_state; }

void Antagonist::NotifyCollision(std::shared_ptr<GameObject> other) {
  auto player_cpt = other->GetComponent(Types::PlayerType);
  if (player_cpt && (last_action != Helpers::Action::ATTACKING)) {
    printf("Push no ataque!\n");
    // Push(new AttackFSM(associated));
  }
}

void Antagonist::Start() {
  last_action = Helpers::Action::IDLE;
  last_direction = Helpers::Direction::NONE;
  state_stack.emplace(new PatrolFSM(associated));
}

void Antagonist::Update(float dt) {
  previous_position = position;

  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

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

      if (!state_stack.empty()) {
        auto& enter_state = state_stack.top();
        enter_state->OnStateEnter();
      }
    }

    if (stored_state != nullptr) {
      state_stack.emplace(stored_state);
      stored_state->OnStateEnter();
      stored_state = nullptr;
    }

    if (state_stack.empty()) {
      state_stack.emplace(new PatrolFSM(associated));
    }

    auto& state = state_stack.top();
    state->Update(dt);
  }

  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;

  // a position está no meio horizontal e no fim vertical do sprite
  // para renderizar, colocamos o xy da box de acordo
  // posição * dimensão do tile - (comprimento da sprite / 2), pois o x fica no
  // meio da sprite
  associated.box.x = position.x * tileDim - sprite->GetWidth() / 2;
  // posição * dimensão do tile - altura da sprite, pois o y fica la embaixo
  associated.box.y = position.y * tileDim - sprite->GetHeight();

  // printf("%s\n", position.ToString().c_str());
}

void Antagonist::Render() {}

void Antagonist::Push(IFSM* s) {
  if (stored_state != nullptr) {
    delete stored_state;
  }
  stored_state = s;
}

bool Antagonist::NearTarget() {
  auto player = GameData::PlayerGameObject->GetComponent(PlayerType);
  if (!player) {
    throw std::runtime_error("Player game object without Player component");
  }

  auto playerCp = std::dynamic_pointer_cast<Player>(player);
  double dist = position.Distance(playerCp->position);
  return dist <= ANTAGONIST_DISTANCE;
}

void Antagonist::AssetsManager(Helpers::Action action) {
  switch (action) {
    case Helpers::Action::IDLE:
      IdleAssetsManager();
      break;
    case Helpers::Action::MOVING:
      MoveAssetsManager(WALKING_WALK_SET);
      break;
    case Helpers::Action::SUSPECTING:
      MoveAssetsManager(SUSPECTING_WALK_SET);
      break;
    case Helpers::Action::CHASING:
      MoveAssetsManager(CHASING_WALK_SET);
      break;
    case Helpers::Action::ATTACKING:
      AttackAssetsManager();
      break;
    default:
      action = Helpers::Action::IDLE;
      IdleAssetsManager();
      break;
  }
  last_action = action;
}

void Antagonist::MoveAssetsManager(std::vector<std::string> set) {
  if (set.size() != 8) {
    throw std::runtime_error("O set de MoveAssetsManager deve ter 8 imagens");
  }

  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sprite");
  }
  Vec2 delta = position - previous_position;

  bool up = delta.y == -1;
  bool down = delta.y == 1;
  bool left = delta.x == -1;
  bool right = delta.x == 1;

  auto direction = Helpers::combine_moves(up, down, left, right);

  if (last_direction == direction) {
    return;
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  switch (direction) {
    case Helpers::Direction::RIGHT:
      sprite->Open(set[0]);
      break;
    case Helpers::Direction::LEFT:
      sprite->Open(set[1]);
      break;
    case Helpers::Direction::UP:
      sprite->Open(set[2]);
      break;
    case Helpers::Direction::DOWN:
      sprite->Open(set[3]);
      break;
    case Helpers::Direction::UPRIGHT:
      sprite->Open(set[4]);
      break;
    case Helpers::Direction::UPLEFT:;
      sprite->Open(set[5]);
      break;
    case Helpers::Direction::DOWNRIGHT:
      sprite->Open(set[6]);
      break;
    case Helpers::Direction::DOWNLEFT:
      sprite->Open(set[7]);
      break;
    default:
      IdleAssetsManager();
      break;
  }

  last_direction = direction;
}

void Antagonist::IdleAssetsManager() {
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

  sound->Open(BREATHING_ANTAGONIST_SOUND);
  sound->Play();

  switch (last_direction) {
    case Helpers::Direction::RIGHT:
      sprite->Open(RIGHT_IDLE_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::LEFT:
      sprite->Open(LEFT_IDLE_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::UP:
      sprite->Open(UP_IDLE_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::DOWN:
      sprite->Open(DOWN_IDLE_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::UPRIGHT:
      sprite->Open(UPRIGHT_IDLE_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::UPLEFT:;
      sprite->Open(UPLEFT_IDLE_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::DOWNRIGHT:
      sprite->Open(DOWNRIGHT_IDLE_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::DOWNLEFT:
      sprite->Open(DOWNLEFT_IDLE_SPRITE_ANTAGONIST);
      break;
    default:
      sprite->Open(DOWN_IDLE_SPRITE_ANTAGONIST);
      break;
  }
}

void Antagonist::AttackAssetsManager() {
  auto spriteCpt = associated.GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("O gameobject do antagonista nao tem sprite");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);

  switch (last_direction) {
    case Helpers::Direction::RIGHT:
      sprite->Open(RIGHT_ATTACK_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::LEFT:
      sprite->Open(LEFT_ATTACK_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::UP:
      sprite->Open(UP_ATTACK_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::DOWN:
      sprite->Open(DOWN_ATTACK_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::UPRIGHT:
      sprite->Open(UPRIGHT_ATTACK_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::UPLEFT:;
      sprite->Open(UPLEFT_ATTACK_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::DOWNRIGHT:
      sprite->Open(DOWNRIGHT_ATTACK_SPRITE_ANTAGONIST);
      break;
    case Helpers::Direction::DOWNLEFT:
      sprite->Open(DOWNLEFT_ATTACK_SPRITE_ANTAGONIST);
      break;
    default:
      sprite->Open(DOWN_IDLE_SPRITE_ANTAGONIST);
      break;
  }
}

bool Antagonist::Is(Types type) const { return type == this->Type; }
