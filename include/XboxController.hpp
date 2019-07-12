#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define INCLUDE_SDL
#include "SDL_include.h"

class XboxController {
 public:
  static int DeadZone;
  static SDL_Joystick* Joystick;
};

#endif
