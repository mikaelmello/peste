#ifndef MENUSTATE_H
#define MENUSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define MAIN_THEME "assets/audio/menu/main_theme.ogg"

#define MENU_BOX_SPRITE "assets/img/menu/open/box.png"
#define MENU_PLAY_BUTTON_SPRITE "assets/img/menu/open/play_button.png"
#define MENU_TUTORIAL_BUTTON_SPRITE "assets/img/menu/open/tutorial_button.png"
#define MENU_CREDITS_BUTTON_SPRITE "assets/img/menu/open/credits_button.png"
#define MENU_QUIT_BUTTON_SPRITE "assets/img/menu/open/quit_button.png"
#define MENU_CURSOR_SPRITE "assets/img/menu/cursor.png"
#define MENU_BACKGROUND_SPRITE "assets/img/menu/menu_background.png"

#include <memory>
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "InputManager.hpp"
#include "Options.hpp"
#include "Sound.hpp"
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
  bool played;

  enum Cursor { play, tutorial, credits, quit };

  const Vec2 position;

  std::shared_ptr<Options> options;

  std::vector<std::shared_ptr<GameObject>> buttons;

  InputManager& im;

  std::shared_ptr<GameObject> background;
};

#endif
