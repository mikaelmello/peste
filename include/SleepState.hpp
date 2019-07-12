#ifndef SLEEPSTATE_H
#define SLEEPSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define WRECKING_SOUND "assets/audio/sound_effects/wrecking_wood.wav"
#define SLEEPING_BG "assets/img/sleeping_background.png"

#include "InputManager.hpp"
#include "Music.hpp"
#include "Sound.hpp"
#include "State.hpp"

class SleepState : public State {
 public:
  SleepState();

  ~SleepState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  Timer pop_timer;

  std::shared_ptr<GameObject> sound_go;
  std::shared_ptr<GameObject> sprite_go;

  InputManager& im;
};

#endif
