#define INCLUDE_SDL
#include "InputManager.hpp"
#include <memory>
#include <unordered_map>
#include "SDL_include.h"
#include "XboxController.hpp"

InputManager InputManager::instance;

InputManager& InputManager::GetInstance() { return instance; }

InputManager::InputManager()
    : quitRequested(false), updateCounter(0), mouseX(0), mouseY(0) {
  ::memset(mouseState, 0, sizeof(bool) * 6);
  ::memset(mouseUpdate, 0, sizeof(int) * 6);
}

InputManager::~InputManager() {}

void InputManager::Update() {
  SDL_Event event;
  updateCounter += 1;

  SDL_GetMouseState(&mouseX, &mouseY);
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quitRequested = true;
    } else if (event.type == SDL_JOYAXISMOTION) {
      if (event.jaxis.which == 0) {
        if (event.jaxis.axis == 0) {
          int newLeft, newRight;
          if (event.jaxis.value < -XboxController::DeadZone) {
            newLeft = 1;
            newRight = 0;
          } else if (event.jaxis.value > XboxController::DeadZone) {
            newLeft = 0;
            newRight = 1;
          } else {
            newLeft = 0;
            newRight = 0;
          }
          if (joyKeyState[JOY_LEFT_KEY] != newLeft) {
            joyKeyState[JOY_LEFT_KEY] = newLeft;
            joyKeyUpdate[JOY_LEFT_KEY] = updateCounter;
          }
          if (joyKeyState[JOY_RIGHT_KEY] != newRight) {
            joyKeyState[JOY_RIGHT_KEY] = newRight;
            joyKeyUpdate[JOY_RIGHT_KEY] = updateCounter;
          }
        }  // Y axis motion
        else if (event.jaxis.axis == 1) {
          int newUp, newDown;
          if (event.jaxis.value < -XboxController::DeadZone) {
            newUp = 1;
            newDown = 0;
          } else if (event.jaxis.value > XboxController::DeadZone) {
            newUp = 0;
            newDown = 1;
          } else {
            newUp = 0;
            newDown = 0;
          }
          if (joyKeyState[JOY_UP_KEY] != newUp) {
            joyKeyState[JOY_UP_KEY] = newUp;
            joyKeyUpdate[JOY_UP_KEY] = updateCounter;
          }
          if (joyKeyState[JOY_DOWN_KEY] != newDown) {
            joyKeyState[JOY_DOWN_KEY] = newDown;
            joyKeyUpdate[JOY_DOWN_KEY] = updateCounter;
          }
        }
      }
    } else if (!event.key.repeat && event.type == SDL_JOYBUTTONDOWN ||
               event.type == SDL_JOYBUTTONUP) {
      joyKeyState[event.jbutton.button] = event.type == SDL_JOYBUTTONUP;
      joyKeyUpdate[event.jbutton.button] = updateCounter;
    } else if (event.type == SDL_MOUSEBUTTONDOWN ||
               event.type == SDL_MOUSEBUTTONUP) {
      mouseState[event.button.button] = event.type == SDL_MOUSEBUTTONDOWN;
      mouseUpdate[event.button.button] = updateCounter;
    } else if (!event.key.repeat &&
               (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)) {
      keyState[event.key.keysym.sym] = event.type == SDL_KEYDOWN;
      keyUpdate[event.key.keysym.sym] = updateCounter;
    }
  }
}

bool InputManager::KeyPress(int key) {
  return keyState[key] && (keyUpdate[key] == updateCounter);
}

bool InputManager::KeyRelease(int key) {
  return !keyState[key] && (keyUpdate[key] == updateCounter);
}

bool InputManager::IsKeyDown(int key) { return keyState[key]; }

bool InputManager::JoyKeyPress(int key) {
  return joyKeyState[key] && (joyKeyUpdate[key] == updateCounter);
}

bool InputManager::IsJoyKeyDown(int key) {
  if (key <= 10) {
    return SDL_JoystickGetButton(XboxController::Joystick, key);
  }
  return joyKeyState[key];
}

bool InputManager::MousePress(int button) const {
  return mouseState[button] && (mouseUpdate[button] == updateCounter);
}

bool InputManager::MouseRelease(int button) const {
  return !mouseState[button] && (mouseUpdate[button] == updateCounter);
}

bool InputManager::IsMouseDown(int button) const { return mouseState[button]; }

int InputManager::GetMouseX() const { return mouseX; }

int InputManager::GetMouseY() const { return mouseY; }

bool InputManager::QuitRequested() const { return quitRequested; }
