#ifndef ROOMSTATE_H
#define ROOMSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Helpers.hpp"
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

 private:
  void LoadFurnitureFirstFloor();

  void LoadFurnitureSecondFloor();

  void LoadFurnitureBasement();

  void LoadDoors();

  void LoadStairs();

  void LoadItems();

  std::shared_ptr<GameObject> ant;

  std::shared_ptr<Music> backgroundSound;

  Helpers::Floor last_known = Helpers::Floor::GROUND_FLOOR;
};

#endif
