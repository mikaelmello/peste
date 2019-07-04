#include "GameData.hpp"
#include <memory>
#include "Item.hpp"
#include "Types.hpp"

GameObject* GameData::PlayerGameObject = nullptr;

std::vector<std::shared_ptr<GameObject>> GameData::PlayerInventory;

void GameData::AddToInventory(std::shared_ptr<GameObject> item) {
  auto item_type_cpt = item->GetComponent(ItemType);
  if (!item_type_cpt) {
    throw std::invalid_argument(
        "Trying to add game object without an item component in the inventory");
  }

  auto item_cpt = std::dynamic_pointer_cast<Item>(item_type_cpt);

  item_cpt->Pickup();
  item->RequestDelete();
  GameData::PlayerInventory.push_back(item);
}
