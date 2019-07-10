#define INCLUDE_SDL
#include <iostream>
#include "Game.hpp"
#include "MenuState.hpp"
#include "SDL_include.h"

int main() {
  try {
    srand(time(NULL));
    auto roomState = new MenuState();
    Game& game = Game::GetInstance();
    game.Push(roomState);
    game.Run();
  } catch (const std::exception& ex) {
    std::cout << "Game crashed\n\n" << std::endl;
    std::cout << "Exception: " << ex.what() << std::endl;
    std::cout << "SDL Error: " << SDL_GetError() << std::endl;
  }

  return 0;
}
