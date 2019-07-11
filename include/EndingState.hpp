#ifndef ENDINGSTATE_H
#define ENDINGSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define BAD_ENDING_SPRITE "assets/img/endings/bad_ending.png"
#define GOOD_ENDING_SPRITE "assets/img/endings/good_ending.png"

#include "Dialog.hpp"
#include "InputManager.hpp"
#include "Music.hpp"
#include "State.hpp"

class EndingState : public State {
 public:
  EndingState(int ending);

  ~EndingState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  int ending;

  SCRIPT_TYPE dialog;

  InputManager& im;
};

#endif
