#ifndef ROOMSTATE_H
#define ROOMSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Music.hpp"
#include "State.hpp"

class RoomState : public State {
 public:
  RoomState();

  ~RoomState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

  void LoadFurnitureFirstFloor();
  void LoadFurnitureSecondFloor();
  void LoadFurnitureBasement();
  void LoadDoors();
  void LoadStairs();
};

#endif
