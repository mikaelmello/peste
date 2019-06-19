#ifndef PAUSESTATE_H
#define PAUSESTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Music.hpp"
#include "State.hpp"

class PauseState : public State {
 public:
  PauseState();

  ~PauseState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();
};

#endif
