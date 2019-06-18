#include "Antagonist.hpp"
#include <iostream>
#include <string>
#include "Collider.hpp"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IdleFSM.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"
#include "Sprite.hpp"

Antagonist::Antagonist(GameObject& associated, Vec2 position)
    : Component(associated), position(position), stored_state(nullptr) {
  Collider* collider = new Collider(associated);
  Sprite* sprite = new Sprite(associated, ANTAGONIST_SPRITE, 4, 0.1);

  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Antagonist::~Antagonist() {}

void Antagonist::NotifyCollision(GameObject& other) {}

void Antagonist::Start() {
  position = Vec2(240, 192);
  state_stack.emplace(new PatrolFSM(associated));
}

void Antagonist::Update(float dt) {
  if (!state_stack.empty()) {
    auto& top_state = state_stack.top();
    if (top_state->PopRequested()) {
      state_stack.pop();
      if (!state_stack.empty()) {
        auto& enter_state = state_stack.top();
        enter_state->OnStateEnter();
      }
    }

    if (stored_state != nullptr) {
      if (!state_stack.empty()) {
        auto& exit_state = state_stack.top();
        exit_state->OnStateExit();
      }
      state_stack.emplace(stored_state);
      stored_state->OnStateEnter();
      stored_state = nullptr;
    }

    // Condicional para tratar eventuais erros lógicos.
    if (state_stack.empty()) {
      printf(
          "Tem algum erro na FSM aí parceiro. Botando o monstro pra "
          "patrulhar.\n");
      state_stack.emplace(new PatrolFSM(associated));
    }

    auto& state = state_stack.top();
    state->Update(dt);
  }

  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;
}

void Antagonist::Render() {}

void Antagonist::Push(IFSM* s) { stored_state = s; }

/*void Antagonist::SpriteManager(Vec2 ant) const {
  std::pair<int, int> delta = {(position - ant).x, (position - ant).y};
  std::vector<std::pair<int, int>> deltas = {
      {-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};

  auto it = std::find(deltas.begin(), deltas.end(), delta);
  int change = 0;
  if (it != deltas.end()) {
    change = std::distance(deltas.begin(), it) + 1;
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(
      associated.GetComponent(GameData::Sprite).lock());
  switch (change) {
    case 0:
      sprite->Open(IDLE_TERRY);
      break;
    case 1:
      sprite->Open(LEFT_WALK_TERRY);
      break;
    case 2:
      sprite->Open(RIGHT_)
  }
}*/

bool Antagonist::Is(GameData::Types type) const { return type == this->Type; }
