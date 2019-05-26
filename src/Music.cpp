#define INCLUDE_SDL
#define INCLUDE_SDL_MIXER
#include "Music.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include "Resources.hpp"
#include "SDL_include.h"

Music::Music() {}

Music::Music(const std::string& file) : Music() { Open(file); }

void Music::Open(const std::string& file) { music = Resources::GetMusic(file); }

void Music::Play(int times) {
  if (!music) {
    throw std::invalid_argument("Can not play music, file not open");
    return;
  }

  int return_code = Mix_PlayMusic(music.get(), times);
  if (return_code != 0) {
    throw std::runtime_error("Could not play music: " +
                             std::string(Mix_GetError()));
  }
}

void Music::Stop(int msTostop) {
  int return_code = Mix_FadeOutMusic(msTostop);
  if (return_code != 1) {
    throw std::runtime_error("Could not stop music: " +
                             std::string(Mix_GetError()));
  }
}

bool Music::IsOpen() const { return (bool)music; }

Music::~Music() {}
