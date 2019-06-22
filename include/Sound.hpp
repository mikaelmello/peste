#ifndef SOUND_H
#define SOUND_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INCLUDE_SDL_MIXER
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "SDL_include.h"
#include "Types.hpp"

class Sound : public Component {
 public:
  Sound(GameObject& associated);

  Sound(GameObject& associated, const std::string& file);

  ~Sound();

  void Open(const std::string& file);

  void Play(int times = 1);

  void Stop();

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  bool IsOpen() const;

  const Types Type = SoundType;

 private:
  std::shared_ptr<Mix_Chunk> chunk;
  int channel;
};

#endif
