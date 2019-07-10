#include "Lore.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "Game.hpp"
#include "GameData.hpp"
#include "InputManager.hpp"
#include "Resources.hpp"
#include "SDL_include.h"
#include "SleepState.hpp"
#include "Sprite.hpp"

bool Lore::NicePerson = false;

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
