#ifndef MENUSTATE_H
#define MENUSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MENU_BACKGROUND_SPRITE "assets/img/menu/menu_background.png"
#define MENU_PLAY_BUTTON_SPRITE "assets/img/menu/play_button.png"
#define MENU_TUTORIAL_BUTTON_SPRITE "assets/img/menu/tutorial_button.png"
#define MENU_CREDITS_BUTTON_SPRITE "assets/img/menu/credits_button.png"
#define MENU_QUIT_BUTTON_SPRITE "assets/img/menu/quit_button.png"
#define MENU_CURSOR_SPRITE "assets/img/menu/cursor.png"

#include "Helpers.hpp"
#include "InputManager.hpp"
#include "Music.hpp"
#include "Options.hpp"
#include "State.hpp"

class MenuState : public State {
 public:
  MenuState();

  ~MenuState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  enum Cursor { play, tutorial, credits, quit };

  const Vec2 position;

  std::shared_ptr<Options> options;

  std::vector<std::shared_ptr<GameObject>> buttons;

  InputManager& im;
};

#endif
