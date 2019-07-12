#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "Game.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "InputManager.hpp"
#include "Resources.hpp"
#include "SDL_include.h"
#include "XboxController.hpp"
Game* Game::instance = nullptr;

Game::Game(const std::string& title, int width, int height)
    : frameStart(0), dt(0), width(width), height(height) {
  if (instance == nullptr) {
    instance = this;
  } else {
    throw std::logic_error(
        "Game constructor called when an instance is already created");
  }
  int return_code;

  return_code = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER |
                         SDL_INIT_JOYSTICK);
  if (return_code != 0) {
    throw std::runtime_error("Failed to init the SDL library");
  }

  // Check for joysticks
  if (SDL_NumJoysticks() < 1) {
    printf("Warning: No joysticks connected!\n");
  } else {
    // Load joystick
    XboxController::Joystick = SDL_JoystickOpen(0);
    if (XboxController::Joystick == NULL) {
      printf("Warning: Unable to open game controller! SDL Error: %s\n",
             SDL_GetError());
    }
  }

  int img_flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
  return_code = IMG_Init(img_flags);
  if (return_code != img_flags) {
    throw std::runtime_error("Failed to init the SDL_Image library");
  }

  return_code = TTF_Init();
  if (return_code != 0) {
    throw std::runtime_error("Failed to init the SDL_TTF library");
  }

  return_code = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
                              MIX_DEFAULT_CHANNELS, 1024);
  if (return_code != 0) {
    throw std::runtime_error(
        "Failed to open the mixer with default parameters");
  }

  int mix_flags = MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG;
  return_code = Mix_Init(mix_flags);
  if (return_code != mix_flags) {
    throw std::runtime_error("Failed to init the SDL_Mixer library");
  }

  Mix_AllocateChannels(32);

  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, width, height, 0);
  if (window == nullptr) {
    throw std::runtime_error("Failed to create a window");
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    throw std::runtime_error(
        "Failed to create a 2D rendering context for the window");
  }

  storedState = nullptr;
}

Game::~Game() {
  if (storedState != nullptr) {
    delete storedState;
  }

  while (!stateStack.empty()) {
    stateStack.pop();
  }

  Resources::ClearImages();
  Resources::ClearMusics();
  Resources::ClearSounds();

  // Close game controller
  SDL_JoystickClose(XboxController::Joystick);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  Mix_CloseAudio();
  TTF_Quit();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
}

Game& Game::GetInstance() {
  if (instance == nullptr) {
    instance = new Game("PESTE", 1024, 768);
  }
  return *instance;
}

State& Game::GetCurrentState() {
  if (stateStack.empty()) {
    throw std::runtime_error("State stack is empty, can't get current state");
  }

  return *(stateStack.top().get());
}

SDL_Renderer* Game::GetRenderer() { return renderer; }

void Game::Push(State* state) { storedState = state; }

void Game::Run() {
  if (storedState == nullptr) {
    throw std::runtime_error("Can not start Game without an initial state");
  }

  stateStack.emplace(storedState);
  storedState = nullptr;

  GetCurrentState().Start();

  while (!(stateStack.empty() || GetCurrentState().QuitRequested())) {
    auto& topState = stateStack.top();
    if (topState->PopRequested()) {
      stateStack.pop();
      if (!stateStack.empty()) {
        auto& resumeState = stateStack.top();
        resumeState->Resume();
      }
    }

    if (storedState != nullptr) {
      if (!stateStack.empty()) {
        auto& pauseState = stateStack.top();
        pauseState->Pause();
      }
      stateStack.emplace(storedState);
      storedState->Start();
      storedState = nullptr;
    }

    if (stateStack.empty()) {
      break;
    }

    auto& state = stateStack.top();

    InputManager::GetInstance().Update();
    CalculateDeltaTime();
    state->Update(GetDeltaTime());
    SDL_RenderClear(renderer);
    state->Render();
    SDL_RenderPresent(renderer);
    // fpsthink();
    // printf("%f\n", framespersecond);
  }

  while (!stateStack.empty()) {
    stateStack.pop();
  }

  Resources::ClearImages();
  Resources::ClearMusics();
  Resources::ClearSounds();
}

void Game::CalculateDeltaTime() {
  int ticks = SDL_GetTicks();
  dt = (ticks - frameStart) / 1000.0f;
  frameStart = ticks;
}

float Game::GetDeltaTime() const { return dt; }

int Game::GetWidth() const { return width; }

int Game::GetHeight() const { return height; }
