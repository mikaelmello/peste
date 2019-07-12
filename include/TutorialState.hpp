#ifndef TUTORIALSTATE_H
#define TUTORIALSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TUTORIAL_BACKGROUND_SPRITE "assets/img/menu/menu_background.png"
#define TUTORIAL_BOX "assets/img/menu/tutorial/tutorialbox.png"
#define TUTORIAL_BOX2 "assets/img/menu/tutorial/tutorialbox2.png"
#define MENU_CURSOR_SPRITE "assets/img/menu/cursor.png"
#define TUTORIALS \
  { TUTORIAL_BOX, TUTORIAL_BOX2 }

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
  int k;
  InputManager& im;

  std::vector<std::string> tutorials;

  std::shared_ptr<GameObject> box;
};

#endif
