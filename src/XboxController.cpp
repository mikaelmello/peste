#include "XboxController.hpp"
#define INCLUDE_SDL
#include "SDL_include.h"

int XboxController::DeadZone = 16000;
SDL_Joystick* XboxController::Joystick = nullptr;
