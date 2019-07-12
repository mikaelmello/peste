#include "GameData.hpp"
#include <memory>
#include "Antagonist.hpp"
#include "Helpers.hpp"
#include "Item.hpp"
#include "Types.hpp"

std::shared_ptr<GameObject> GameData::MonsterGameObject;
std::shared_ptr<GameObject> GameData::PlayerGameObject;
std::shared_ptr<GameObject> GameData::DialogGameObject;
std::shared_ptr<GameObject> GameData::TerryBedGameObject;
std::shared_ptr<GameObject> GameData::TerryGameObject;

std::vector<std::shared_ptr<GameObject>> GameData::PlayerInventory;

bool GameData::transformed_monster_in_terry = false;
bool GameData::canUseLamp = false;
bool GameData::player_was_hit = false;
bool GameData::player_is_hidden = false;
bool GameData::can_visit_basement = false;
bool GameData::player_in_safehouse = false;

Rect GameData::Basement = Rect(0, 8000, 5000, 4000);
Rect GameData::Floor1 = Rect(0, 38, 5000, 3500);
Rect GameData::Floor2 = Rect(0, 4100, 5000, 3900);
Rect GameData::SafeHouse = Rect(98, 6721, 1080, 1110);

int GameData::InventoryPage = 0;
bool GameData::got_key1 = false;

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
  if (!GameData::DialogGameObject) {
    throw std::runtime_error("Nao tem dialog game object pra ter dialog");
  }
  auto dialogCpt = GameData::DialogGameObject->GetComponent(DialogType);
  if (!dialogCpt) {
    throw std::invalid_argument("Trying to init script without dialog????");
  }

  auto dialog = std::dynamic_pointer_cast<Dialog>(dialogCpt);

  dialog->SetScript(script);
}

bool GameData::CanUseLamp() {
  if (canUseLamp) {
    return true;
  }

  unsigned c = 0;
  for (auto i : PlayerInventory) {
    auto item_cpt = i->GetComponent(ItemType);
    if (!item_cpt) {
      throw std::runtime_error("sem item em GameData::CanVisitBasement");
    }
    auto item = std::dynamic_pointer_cast<Item>(item_cpt);

    if (item->GetName() == "Isqueiro" || item->GetName() == "Lamparina" ||
        item->GetName() == "Óleo") {
      c++;
    }
  }

  canUseLamp = c >= 3;
  return canUseLamp;
}

bool GameData::HasCrowbar() {
  for (auto i : PlayerInventory) {
    auto item_cpt = i->GetComponent(ItemType);
    if (!item_cpt) {
      throw std::runtime_error("sem item em GameData::CanDefendYourself");
    }
    auto item = std::dynamic_pointer_cast<Item>(item_cpt);

    if (item->GetName() == "Pé de cabra") return true;
  }
  return false;
}

void GameData::LoadAntagonistPaths() {
  if (!MonsterGameObject) {
    return;
  }

  std::vector<Vec2> path;

  switch (GameData::PlayerFloor()) {
    case Helpers::Floor::BASEMENT:
      path = {{215, 1092}};
      break;
    case Helpers::Floor::GROUND_FLOOR:
      path = {{263, 297}, {361, 297}, {351, 394}, {89, 383},
              {203, 296}, {268, 202}, {253, 93},  {82, 118},
              {82, 243},  {40, 76},   {197, 80},  {276, 202}};
      break;
    case Helpers::Floor::FIRST_FLOOR:
      path = {{347, 579}, {175, 582}, {169, 698}, {54, 696},  {66, 613},
              {106, 698}, {93, 784},  {180, 713}, {187, 605}, {286, 609}};
      break;
    default:
      path = {{263, 297}};
      break;
  }

  auto ant_cpt = MonsterGameObject->GetComponent(AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("sem antagonista em RoomState::LoadAntagonist");
  }
  auto antagonista = std::dynamic_pointer_cast<Antagonist>(ant_cpt);
  antagonista->NewPatrolPath(path);
}

Helpers::Floor GameData::PlayerFloor() {
  auto playerPos = PlayerGameObject->box.Center();
  if (Basement.Contains(playerPos)) {
    return Helpers::Floor::BASEMENT;
  }
  if (Floor1.Contains(playerPos)) {
    return Helpers::Floor::GROUND_FLOOR;
  }
  if (Floor2.Contains(playerPos)) {
    return Helpers::Floor::FIRST_FLOOR;
  }
  return Helpers::Floor::NO_FLOOR;
}

Helpers::Floor GameData::MonsterFloor() {
  auto monsterPos = MonsterGameObject->box.Center();
  if (Basement.Contains(monsterPos)) {
    return Helpers::Floor::BASEMENT;
  }
  if (Floor1.Contains(monsterPos)) {
    return Helpers::Floor::GROUND_FLOOR;
  }
  if (Floor2.Contains(monsterPos)) {
    return Helpers::Floor::FIRST_FLOOR;
  }
  return Helpers::Floor::NO_FLOOR;
}
