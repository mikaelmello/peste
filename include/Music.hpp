#ifndef MUSIC_H
#define MUSIC_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INCLUDE_SDL_MIXER
#include <memory>
#include <string>
#include "SDL_include.h"

class Music {
 public:
  Music();

  Music(const std::string& file);

  ~Music();

  void Play(int times = -1);
  void Stop(int msToStop = 1500);
  void Open(const std::string& file);
  bool IsOpen() const;

 private:
  std::shared_ptr<Mix_Music> music;
};

#endif
