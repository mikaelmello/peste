#include "SuspectFSM.hpp"

#include "Antagonist.hpp"
#include "Game.hpp"
#include "Pathfinder.hpp"
#include "PatrolFSM.hpp"
#include "Player.hpp"
#include "PursuitFSM.hpp"
#include "Types.hpp"

SuspectFSM::SuspectFSM(GameObject& object)
    : IFSM(object, SUSPECT_SPEED), rage_bias(0), cooldown_lock(false) {
  stack_original_size = Antagonist::paths.size();

  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error("sem antagonist em SuspectFSM::SuspectFSM");
  }
  ant = std::dynamic_pointer_cast<Antagonist>(antCpt);
  initial = ant.lock()->position;
}

SuspectFSM::~SuspectFSM() {}

void SuspectFSM::OnStateEnter() {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  if (rage_bias == RAGE_NUMERIC_LIMIT) rage_bias--;

  if (ant.lock()->NearTarget()) {
    Walkable w = GetWalkable(object, *GameData::PlayerGameObject);

    if (w.can_walk) {
      auto pf = Pathfinder::Astar(object, tilemap);
      Antagonist::paths.emplace(0, pf.Run(initial, w.walkable));

    } else {
      pop_requested = true;
    }
  }
}

void SuspectFSM::OnStateExecution(float dt) {
  cooldown_lock = !(cooldown.Get() >= 0.5);

  bool go_idle = UpdatePosition(dt);

  if (rage_bias == RAGE_NUMERIC_LIMIT && !cooldown_lock) {
    ant.lock()->Push(new PursuitFSM(object));
    cooldown.Restart();
    cooldown_lock = true;
  }

  if (go_idle) {
    ant.lock()->AssetsManager(Helpers::Action::IDLE);
    return;
  }

  ant.lock()->AssetsManager(Helpers::Action::SUSPECTING);
}

void SuspectFSM::OnStateExit() {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  while (stack_original_size < Antagonist::paths.size()) {
    Antagonist::paths.pop();
  }

  Vec2 current = ant.lock()->position;

  auto pf = Pathfinder::Astar(object, tilemap);
  pf.ToggleMaxSearch();

  auto return_path = pf.Run(current, initial);

  Antagonist::paths.emplace(0, return_path);
}

void SuspectFSM::Update(float dt) {
  auto bound_value = [](int x) {
    return std::max(std::min(x, RAGE_NUMERIC_LIMIT), NO_RAGE_BIAS);
  };

  if (bias_update_timer.Get() > 3 * dt) {
    bool near = ant.lock()->NearTarget();
    rage_bias = near ? bound_value(rage_bias + 1) : bound_value(rage_bias - 1);
    bias_update_timer.Restart();
  }

  OnStateExecution(dt);
  bias_update_timer.Update(dt);

  if (cooldown_lock) {
    cooldown.Update(dt);
  }

  if (rage_bias == NO_RAGE_BIAS) {
    pop_requested = pop_request_timer.Get() >= POP_REQUEST_TIME;
    pop_request_timer.Update(dt);
  } else {
    pop_request_timer.Restart();
  }
}
