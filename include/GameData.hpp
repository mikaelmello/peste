#ifndef GAMEDATA_H
#define GAMEDATA_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <vector>
#include "Dialog.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"

class GameData {
 public:
  static std::shared_ptr<GameObject> PlayerGameObject;

  static std::vector<std::shared_ptr<GameObject>> PlayerInventory;
  static bool AddToInventory(std::shared_ptr<GameObject> item);
  static int InventoryPage;

  static std::shared_ptr<GameObject> DialogGameObject;
  static void InitDialog(SCRIPT_TYPE script);

  static bool player_was_hit;

  static bool player_is_hidden;

  static Helpers::Floor hope_is_in;

  static std::shared_ptr<GameObject> TerryBedGameObject;
  static std::shared_ptr<GameObject> TerryGameObject;

  static bool CanUseLamp();

 private:
  static bool can_visit_basement;
};

#endif
