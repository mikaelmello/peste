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
  static std::shared_ptr<GameObject> MonsterGameObject;

  static std::vector<std::shared_ptr<GameObject>> PlayerInventory;
  static bool AddToInventory(std::shared_ptr<GameObject> item);
  static int InventoryPage;

  static std::shared_ptr<GameObject> DialogGameObject;
  static void InitDialog(SCRIPT_TYPE script);

  static bool player_was_hit;

  static bool player_is_hidden;
  static bool player_in_safehouse;

  static bool got_key1;
  static std::shared_ptr<GameObject> TerryBedGameObject;
  static std::shared_ptr<GameObject> TerryGameObject;

  static bool CanUseLamp();
  static void LoadAntagonistPaths();
  static Helpers::Floor PlayerFloor();
  static Helpers::Floor MonsterFloor();

  static Rect Basement;
  static Rect Floor1;
  static Rect Floor2;
  static Rect SafeHouse;

  static bool HasCrowbar();

 private:
  static bool can_visit_basement;
};

#endif
