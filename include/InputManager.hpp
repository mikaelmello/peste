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
#define LSHIFT_KEY SDLK_LSHIFT
#define W_KEY SDLK_w
#define A_KEY SDLK_a
#define S_KEY SDLK_s
#define D_KEY SDLK_d
#define I_KEY SDLK_i
#define X_KEY SDLK_x

enum JoyKeys {
  JOY_DOWN_KEY = 55,
  JOY_UP_KEY = 56,
  JOY_LEFT_KEY = 57,
  JOY_RIGHT_KEY = 58,
  JOY_A_KEY = 0,
  JOY_B_KEY = 1,
  JOY_X_KEY = 2,
  JOY_Y_KEY = 3,
  JOY_LB_KEY = 4,
  JOY_RB_KEY = 5,
  JOY_SELECT_KEY = 6,
  JOY_START_KEY = 7,
};

class InputManager {
 public:
  void Update();

  bool KeyPress(int key);
  bool KeyRelease(int key);
  bool IsKeyDown(int key);

  bool JoyKeyPress(int key);
  bool IsJoyKeyDown(int key);

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

  std::unordered_map<int, bool> joyKeyState;
  std::unordered_map<int, int> joyKeyUpdate;

  bool quitRequested;
  int updateCounter;

  int mouseX;
  int mouseY;
};

#endif
