#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define CREDIT_BACKGROUND_SPRITE "assets/img/temp_credit_bg.jpg"

#include "InputManager.hpp"
#include "Music.hpp"
#include "State.hpp"

class CreditsState : public State {
 public:
  CreditsState();

  ~CreditsState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  InputManager& im;
};

#endif
