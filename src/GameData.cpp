#include "GameData.hpp"
#include <memory>
#include "Types.hpp"

GameObject* GameData::PlayerGameObject = nullptr;

std::vector<std::shared_ptr<GameObject>> GameData::PlayerInventory;

void GameData::AddToInventory(std::shared_ptr<GameObject> item) {
  auto item_cpt = item->GetComponent(ItemType).lock();
  if (!item_cpt) {
    throw new std::invalid_argument(
        "Trying to add game object without an item component in the inventory");
  }

  GameData::PlayerInventory.push_back(item);
}
