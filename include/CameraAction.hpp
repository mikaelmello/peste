#ifndef CAMERAACTION_H
#define CAMERAACTION_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Timer.hpp"
#include "Vec2.hpp"

class CameraAction {
 public:
  static void Start(Vec2 source, Vec2 target);

  static bool IsRunning();

  static void Update(float dt);

 private:
  static void Stop();

  static bool isRunning;

  static float speed;

  static float angle;

  static float distanceLeft;

  static Timer timer;
};

#endif
