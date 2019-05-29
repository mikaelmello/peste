#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
};

#endif
