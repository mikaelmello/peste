#define INCLUDE_SDL
#include <iostream>
#include "EnemyState.hpp"
#include "Game.hpp"
#include "SDL_include.h"

int main(int argc, char** argv) {
  try {
    auto roomState = new EnemyState();
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
