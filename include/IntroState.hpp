#ifndef INTROSTATE_H
#define INTROSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Music.hpp"
#include "State.hpp"

class IntroState : public State {
 public:
  IntroState();

  ~IntroState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();
};

#endif
