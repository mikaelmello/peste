#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INCLUDE_SDL
#include <string>
#include <unordered_map>
#include "SDL_include.h"

#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN
#define SPACE_BAR_KEY SDLK_SPACE
#define ESCAPE_KEY SDLK_ESCAPE
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT
#define RIGHT_MOUSE_BUTTON SDL_BUTTON_RIGHT
#define ENTER_KEY SDLK_RETURN
#define W_KEY SDLK_w
#define A_KEY SDLK_a
#define S_KEY SDLK_s
#define D_KEY SDLK_d
#define I_KEY SDLK_i

class InputManager {
 public:
  void Update();

  bool KeyPress(int key);
  bool KeyRelease(int key);
  bool IsKeyDown(int key);

  bool MousePress(int button) const;
  bool MouseRelease(int button) const;
  bool IsMouseDown(int button) const;

  int GetMouseX() const;
  int GetMouseY() const;

  bool QuitRequested() const;

  static InputManager& GetInstance();

 private:
  static InputManager instance;

  InputManager();

  ~InputManager();

  bool mouseState[6];
  int mouseUpdate[6];

  std::unordered_map<int, bool> keyState;
  std::unordered_map<int, int> keyUpdate;

  bool quitRequested;
  int updateCounter;

  int mouseX;
  int mouseY;
};

#endif
