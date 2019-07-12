#ifndef CAUGHTSTATE_H
#define CAUGHTSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "InputManager.hpp"
#include "Music.hpp"
#include "State.hpp"

class CaughtState : public State {
 public:
  CaughtState();

  ~CaughtState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  InputManager& im;

  Timer pop_timer;
};

#endif
