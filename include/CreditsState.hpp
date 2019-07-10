#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define CREDIT_BACKGROUND_SPRITE "assets/img/menu/menu_background.png"
#define BOX_CODE "assets/img/menu/credits/code.png"
#define BOX_DESIGN "assets/img/menu/credits/design.png"
#define BOX_MUSIC "assets/img/menu/credits/music.png"
#define MENU_CURSOR_SPRITE "assets/img/menu/cursor.png"
#define CREDITS \
  { BOX_CODE, BOX_DESIGN, BOX_MUSIC }

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

  unsigned k;

  std::vector<std::string> credits;

  std::shared_ptr<GameObject> box;
};

#endif
