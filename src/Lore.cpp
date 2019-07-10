#include "Lore.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "InputManager.hpp"
#include "Resources.hpp"
#include "SDL_include.h"

Lore* Lore::instance = nullptr;

Lore::Lore() {
  if (instance == nullptr) {
    instance = this;
  } else {
    throw std::logic_error(
        "Lore constructor called when an instance is already created");
  }
}

Lore::~Lore() {}

Lore& Lore::GetInstance() {
  if (instance == nullptr) {
    instance = new Lore();
  }
  return *instance;
}
