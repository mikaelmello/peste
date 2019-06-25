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
  Sprite* sprite = new Sprite(associated, ANTAGONIST_SPRITE, 4, 0.125);
  Collider* collider =
      new Collider(associated, {0.6, 0.15}, {0, sprite->GetHeight() * 0.45});

  associated.AddComponent(collider);
  associated.AddComponent(sprite);
}

Antagonist::~Antagonist() { delete stored_state; }

void Antagonist::NotifyCollision(GameObject& other) {}

void Antagonist::Start() { state_stack.emplace(new PatrolFSM(associated)); }

void Antagonist::Update(float dt) {
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
      if (!state_stack.empty()) {
        auto& exit_state = state_stack.top();
        exit_state->OnStateExit();
      }
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

  auto tilemap = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  int tileDim = tilemap->GetLogicalTileDimension();

  associated.box.x = position.x * tileDim;
  associated.box.y = position.y * tileDim;
}

void Antagonist::Render() {}

void Antagonist::Push(IFSM* s) {
  if (stored_state != nullptr) {
    delete stored_state;
  }
  stored_state = s;
}

bool Antagonist::NearTarget() {
  double dist = position.Distance(GameData::hope_position);
  return dist <= ANTAGONIST_DISTANCE;
}

bool Antagonist::Is(GameData::Types type) const { return type == this->Type; }
