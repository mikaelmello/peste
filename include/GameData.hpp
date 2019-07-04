#ifndef GAMEDATA_H
#define GAMEDATA_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <vector>
#include "GameObject.hpp"

class GameData {
 public:
  static GameObject* PlayerGameObject;
  static std::vector<std::shared_ptr<GameObject>> PlayerInventory;

  static bool player_was_hit;

  static void AddToInventory(std::shared_ptr<GameObject> item);
};

#endif
