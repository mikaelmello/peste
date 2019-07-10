#include "GameData.hpp"
#include <memory>
#include "Helpers.hpp"
#include "Item.hpp"
#include "Types.hpp"

std::shared_ptr<GameObject> GameData::PlayerGameObject;
std::shared_ptr<GameObject> GameData::DialogGameObject;

std::vector<std::shared_ptr<GameObject>> GameData::PlayerInventory;

bool GameData::player_was_hit = false;
bool GameData::player_is_hidden = false;
int GameData::InventoryPage = 0;

Helpers::Floor GameData::hope_is_in = Helpers::Floor::GROUND_FLOOR;

bool GameData::AddToInventory(std::shared_ptr<GameObject> item) {
  auto item_type_cpt = item->GetComponent(ItemType);
  if (!item_type_cpt) {
    throw std::invalid_argument(
        "Trying to add game object without an item component in the inventory");
  }

  auto item_cpt = std::dynamic_pointer_cast<Item>(item_type_cpt);

  if (GameData::PlayerInventory.size() >= 18) {
    return false;
  }

  item_cpt->Pickup();
  item->RequestDelete();
  GameData::PlayerInventory.push_back(item);
  return true;
}

void GameData::InitDialog(SCRIPT_TYPE script) {
  auto dialogCpt = GameData::DialogGameObject->GetComponent(DialogType);
  if (!dialogCpt) {
    throw std::invalid_argument("Trying to init script without dialog????");
  }

  auto dialog = std::dynamic_pointer_cast<Dialog>(dialogCpt);

  dialog->SetScript(script);
}
