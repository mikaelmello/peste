#include "PursuitFSM.hpp"
#include "Antagonist.hpp"
#include "Collider.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Types.hpp"

PursuitFSM::PursuitFSM(GameObject& object) : IFSM(object) {
  auto tile_map = Game::GetInstance().GetCurrentState().GetCurrentTileMap();
  pf = std::unique_ptr<Pathfinder::Astar>(
      new Pathfinder::Astar(object, tile_map));
}

PursuitFSM::~PursuitFSM() {}

void PursuitFSM::OnStateEnter() {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  auto antagonist_cpt = object.GetComponent(AntagonistType);
  if (!antagonist_cpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a PursuitFSM");
  }

  auto player_cpt = GameData::PlayerGameObject->GetComponent(PlayerType);
  if (!player_cpt) {
    throw std::runtime_error("Player game object without Player component");
  }

  auto player_col_cpt = GameData::PlayerGameObject->GetComponent(ColliderType);
  if (!player_col_cpt) {
    throw std::runtime_error("player GameObject without Collider");
  }

  auto ant = std::dynamic_pointer_cast<Antagonist>(antagonist_cpt);
  auto player = std::dynamic_pointer_cast<Player>(player_cpt);

  Vec2 walkable;
  bool can_walk = Helpers::CanWalk(object, player->position);

  if (can_walk) {
    walkable = player->position;
  } else {
    auto player_collider = std::dynamic_pointer_cast<Collider>(player_col_cpt);
    int tile_dim = tilemap->GetLogicalTileDimension();

    int cells_width = round(player_collider->box.w / tile_dim);
    int cells_height = round(player_collider->box.h / tile_dim);
    if (cells_width % 2 == 1) cells_width--;

    int x = player->position.x - cells_width / 2;
    int y = player->position.y - cells_height;

    for (int i = x; i < (x + cells_width) && !can_walk; i++) {
      for (int j = y; j < (y + cells_height) && !can_walk; j++) {
        Vec2 test(i, j);
        can_walk = Helpers::CanWalk(object, test);
        if (can_walk) walkable = test;
      }
    }
  }

  if (can_walk) {
    try {
      auto pursuit_path = pf->Run(ant->position, walkable);
      path = {0, pursuit_path};

    } catch (const std::exception& ex) {
      printf("%s\n", ex.what());
      pop_requested = true;
    }
  } else {
    pop_requested = true;
  }
}

void PursuitFSM::OnStateExecution() {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a PursuitFSM em Execution");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (path.first < path.second.size()) {
    ant->position = path.second[path.first];
  }

  ant->AssetsManager(Helpers::Action::CHASING);
}

void PursuitFSM::OnStateExit() {}

void PursuitFSM::Update(float dt) {
  auto antCpt = object.GetComponent(AntagonistType);
  if (!antCpt) {
    throw std::runtime_error(
        "Nao tem antagonista no objeto passado para a PursuitFSM em Update");
  }
  auto ant = std::dynamic_pointer_cast<Antagonist>(antCpt);

  if (timer.Get() >= 1.0f) {
    OnStateEnter();
    timer.Restart();
  }

  OnStateExecution();

  if (path.first < path.second.size() - 1) {
    path.first++;
  }

  if (!ant->NearTarget()) {
    pop_requested = true;
  }

  timer.Update(dt);
}
