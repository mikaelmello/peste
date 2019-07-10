#ifndef TUTORIALSTATE_H
#define TUTORIALSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TUTORIAL_BACKGROUND_SPRITE "assets/img/menu/menu_background.png"
#define TUTORIAL_BOX "assets/img/menu/tutorial/tutorial_box.png"

#include "InputManager.hpp"
#include "Music.hpp"
#include "State.hpp"

class TutorialState : public State {
 public:
  TutorialState();

  ~TutorialState();

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
