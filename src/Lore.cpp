#include "Lore.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "Antagonist.hpp"
#include "CameraAction.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "InputManager.hpp"
#include "Resources.hpp"
#include "SDL_include.h"
#include "SleepState.hpp"
#include "Sprite.hpp"

bool Lore::NicePerson = false;
bool Lore::HasEnteredMasterBedroom = false;
int Lore::Slept = 0;

void Lore::Sleep() {
  if (!GameData::TerryBedGameObject) {
    throw std::runtime_error("Cama do terry nao setada no gamedata");
  }
  if (!GameData::TerryGameObject) {
    throw std::runtime_error("Terry nao setado no gamedata");
  }

  auto terryBedSpriteCpt =
      GameData::TerryBedGameObject->GetComponent(SpriteType);

  if (!terryBedSpriteCpt) {
    throw std::runtime_error("Cama do terry sem sprite");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(terryBedSpriteCpt);
  sprite->Open("assets/img/furniture/bedDestroyed.png");

  GameData::TerryGameObject->RequestDelete();
  GameData::TerryGameObject.reset();

  auto& game = Game::GetInstance();
  game.Push(new SleepState());
}

void Lore::FirstMonsterSpawn() {
  auto& state = Game::GetInstance().GetCurrentState();
  auto ant = new GameObject(100);
  Antagonist* antagonist = new Antagonist(*ant, {{347, 579}});
  ant->AddComponent(antagonist);
  GameData::MonsterGameObject = state.AddObject(ant);
  GameData::LoadAntagonistPaths();

  auto playerPos = GameData::PlayerGameObject->box.Center();
  auto monsterPos = GameData::MonsterGameObject->box.Center();
  CameraAction::Start(playerPos, monsterPos);
}
