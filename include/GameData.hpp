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
  static std::shared_ptr<GameObject> PlayerGameObject;
  static std::vector<std::shared_ptr<GameObject>> PlayerInventory;

  static bool AddToInventory(std::shared_ptr<GameObject> item);

  static bool player_was_hit;

  static bool player_is_hidden;

  static int InventoryPage;
};

#endif
