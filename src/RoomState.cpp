#include "RoomState.hpp"
#include <iostream>
#include "Antagonist.hpp"
#include "Camera.hpp"
#include "CameraFollower.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Dialog.hpp"
#include "Door.hpp"
#include "Furniture.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "InventoryState.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "Stairs.hpp"
#include "Terry.hpp"
#include "TileMap.hpp"
#include "Types.hpp"

#define IMG_PATH "assets/img/furniture/"

RoomState::RoomState() {
  last_known = Helpers::Floor::GROUND_FLOOR;
  GameObject* bckgGo = new GameObject(3);
  Sprite* bckgSprite = new Sprite(*bckgGo, "assets/img/black.jpg");
  bckgGo->AddComponent(new CameraFollower(*bckgGo, {512, 334}));
  bckgGo->AddComponent(bckgSprite);
  objects.emplace_back(bckgGo);

  GameObject* mapGo = new GameObject(4);
  currentTileMap = new TileMap(*mapGo, "assets/map/map.json");
  currentTileMap->SetParallax(1, 0, 0);
  mapGo->AddComponent(currentTileMap);
  objects.emplace_back(mapGo);
}

RoomState::~RoomState() {
  Camera::Unfollow();
  Camera::pos = {0, 0};
}

void RoomState::Update(float dt) {
  std::shared_ptr<Collider> collider;
  std::vector<std::shared_ptr<Collider>> colliders;

  if (quitRequested || popRequested) {
    return;
  }

  Camera::Update(dt);

  InputManager& im = InputManager::GetInstance();
  quitRequested |= im.QuitRequested();

  if (GameData::DialogGameObject->IsRendering()) {
    GameData::DialogGameObject->Update(dt);
    return;
  }

  if (im.KeyPress(ESCAPE_KEY)) {
    Game::GetInstance().Push(new InventoryState());
  }

  for (auto i = objects.begin(); i != objects.end();) {
    if ((*i)->IsDead()) {
      i = objects.erase(i);
    } else {
      i++;
    }
  }

  for (auto i = objects.begin(); i != objects.end(); i++) {
    auto aux = i;
    for (auto j = ++aux; j != objects.end(); j++) {
      auto go1 = *i;
      auto go2 = *j;

      auto colliderSp1 = go1->GetComponent(ColliderType);
      auto colliderSp2 = go2->GetComponent(ColliderType);

      if (!colliderSp1 || !colliderSp2) {
        continue;
      }

      auto collider1 = std::dynamic_pointer_cast<Collider>(colliderSp1);
      auto collider2 = std::dynamic_pointer_cast<Collider>(colliderSp2);

      float radDeg1 = Helpers::deg_to_rad(go1->angleDeg);
      float radDeg2 = Helpers::deg_to_rad(go2->angleDeg);
      bool collides = Collision::IsColliding(collider1->box, collider2->box,
                                             radDeg1, radDeg2);
      if (collides) {
        go1->NotifyCollision(go2);
        go2->NotifyCollision(go1);
      }
    }
  }

  if (GameData::hope_is_in != last_known) {
    // LoadAntagonist();
    last_known = GameData::hope_is_in;
  }

  UpdateArray(dt);
}

void RoomState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void RoomState::Pause() {
  Camera::Unfollow();
  Camera::pos = {0, 0};
}

void RoomState::Resume() { Camera::Follow(GameData::PlayerGameObject.get()); }

void RoomState::LoadAssets() {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  LoadFurnitureFirstFloor();
  LoadFurnitureSecondFloor();
  LoadFurnitureBasement();
  LoadStairs();
  LoadItems();

  tilemap->MergeWalkable();
  LoadDoors();

  auto dialogGo = std::make_shared<GameObject>(1500);
  auto dialog = new Dialog(*dialogGo);
  dialogGo->AddComponent(dialog);
  objects.push_back(dialogGo);
  GameData::DialogGameObject = dialogGo;

  auto playerGo = std::make_shared<GameObject>(10);
  Player* player = new Player(*playerGo, currentTileMap->GetInitialPosition());
  playerGo->AddComponent(player);
  objects.push_back(playerGo);
  GameData::PlayerGameObject = playerGo;

  // std::cout << objects.size() << std::endl;

  // ant = std::make_shared<GameObject>(11);
  // Antagonist* antagonist = new Antagonist(*ant.get(), {{263, 297}});
  // ant->AddComponent(antagonist);
  // objects.push_back(ant);
  // LoadAntagonist();

  Camera::Follow(playerGo.get());

  SortObjects();
}

void RoomState::Render() { RenderArray(); }

void RoomState::LoadFurnitureFirstFloor() {
  GameObject* furnitureGo = new GameObject(6);
  Furniture* furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/cabinet.png", {53, 340},
                    Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  // begin piano room
  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/piano.png",
                            {258, 314}, Helpers::Interaction::PLAY);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/piano_stool.png",
                    {265, 352}, Helpers::Interaction::NOTHING, true);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/sofa_front.png",
                            {183, 305}, Helpers::Interaction::NOTHING, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/sofa_front.png",
                            {324, 305}, Helpers::Interaction::NOTHING, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/misc_shelf.png",
                            {150, 238}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/armchair.png",
                            {306, 258}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/armchair.png",
                            {359, 258}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end piano room

  // begin kitchen
  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/table_big.png",
                            {305, 64}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/cabinet.png",
                            {257, 36}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/pantry3.png",
                            {66, 174}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/pantry1.png",
                            {99, 174}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/kitchen.png",
                            {16, 20}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/kitchen2.png",
                            {16, 55}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/kitchen_isle.png",
                    {50, 80}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end kitchen
}

void RoomState::LoadFurnitureSecondFloor() {
  // begin bedroom 1
  auto furnitureGo = new GameObject(7);
  auto furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/cabinet2.png",
                    {22, 535}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bed2.png",
                            {91, 545}, Helpers::Interaction::HIDE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo,
                            "assets/img/furniture/bedside_plus_abajur.png",
                            {73, 555}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo,
                            "assets/img/furniture/bedside_plus_abajur.png",
                            {126, 555}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end bedroom1

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/clock.png",
                            {210, 532}, Helpers::Interaction::LOOK, true,
                            "Parece que não funciona...");
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  // begin library
  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo,
                            "assets/img/furniture/bedside_plus_armchairs.png",
                            {220, 660}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/desk.png",
                            {346, 652}, Helpers::Interaction::LOOK, true,
                            "Acho melhor não mexer no que não é meu...");
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bookshelf3.png",
                            {334, 837}, Helpers::Interaction::NOTHING, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bookshelf2.png",
                            {334, 777}, Helpers::Interaction::LOOK, false,
                            "Nada interessante..");
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bookshelf1.png",
                            {206, 717}, Helpers::Interaction::LOOK, false,
                            "Alguns livros de medicina...");
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bookshelf3.png",
                            {334, 717}, Helpers::Interaction::NOTHING, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/bookshelf3.png",
                    {206, 777}, Helpers::Interaction::LOOK, false,
                    "Esses livros de hitória falam sobre a Idade Média...");
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/table.png",
                            {233, 855}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end library

  // begin bedroom terry

  auto terryGo = std::make_shared<GameObject>(8);
  Terry* terry = new Terry(*terryGo, {33, 750});
  terryGo->AddComponent(terry);
  GameData::TerryGameObject = terryGo;
  objects.push_back(terryGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/wardrobe.png",
                            {118, 719}, Helpers::Interaction::HIDE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  auto terryBedGo = std::make_shared<GameObject>(7);
  furniture = new Furniture(*terryBedGo, "assets/img/furniture/bedTerry.png",
                            {55, 736}, Helpers::Interaction::HIDE);
  terryBedGo->AddComponent(furniture);
  GameData::TerryBedGameObject = terryBedGo;
  objects.push_back(terryBedGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/toy2.png",
                            {37, 817}, Helpers::Interaction::LOOK, false,
                            "Que brinquedo antigo...");
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/toy3.png",
                            {52, 807}, Helpers::Interaction::NOTHING, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end bedroom terry

  // begin last bedroom
  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/cabinet.png",
                            {35, 870}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bedgreen.png",
                            {69, 867}, Helpers::Interaction::SLEEP);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/bedside_table.png",
                    {90, 873}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end last bedroom
}

void RoomState::LoadFurnitureBasement() {
  // begin first room
  auto furnitureGo = new GameObject(7);
  auto furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/wheelchair.png",
                    {178, 1059}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bedPacient.png",
                            {254, 1059}, Helpers::Interaction::HIDE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end first room

  // begin second room
  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bedPacient.png",
                            {300, 1059}, Helpers::Interaction::HIDE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/serum.png",
                            {290, 1052}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableRight.png",
                    {384, 1063}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/microscope2.png",
                    {386, 1063}, Helpers::Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end second room

  // begin lab
  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableFront.png",
                    {47, 1145}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/lab_stuff.png",
                            {46, 1146}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/equipament.png",
                            {60, 1144}, Helpers::Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/microscope2.png",
                    {74, 1144}, Helpers::Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableFront.png",
                    {107, 1145}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/oldpc.png",
                            {123, 1141}, Helpers::Interaction::LOOK, true,
                            "Parece que estavam avancando nos teste da cura da "
                            "Peste. Mas eu não conheco esses métodos...");
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bookpile.png",
                            {110, 1145}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableLeft.png",
                    {17, 1170}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/microscope1.png",
                    {19, 1174}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/papers.png",
                            {19, 1190}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/chairSteel.png",
                            {174, 1160}, Helpers::Interaction::NOTHING, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/chairSteel.png",
                            {225, 1221}, Helpers::Interaction::NOTHING, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableRight.png",
                    {240, 1217}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableRight.png",
                    {240, 1242}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/openBook.png",
                            {242, 1221}, Helpers::Interaction::NOTHING);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end lab
}

void RoomState::LoadDoors() {
  // begin ground floor
  auto doorGo = new GameObject(8);
  auto door =
      new Door(*doorGo, Helpers::Direction::UP, {256, 224}, false, true);
  doorGo->AddComponent(door);
  objects.emplace_back(doorGo);

  doorGo = new GameObject(8);
  door = new Door(*doorGo, Helpers::Direction::UP, {256, 113}, false, true,
                  Helpers::KeyType::KEY1);
  doorGo->AddComponent(door);
  objects.emplace_back(doorGo);
  // end ground floor

  // begin first floor
  doorGo = new GameObject(8);
  door = new Door(*doorGo, Helpers::Direction::UP, {288, 624}, true, true);
  doorGo->AddComponent(door);
  objects.emplace_back(doorGo);

  doorGo = new GameObject(8);
  door = new Door(*doorGo, Helpers::Direction::UP, {48, 640}, false, false,
                  Helpers::KeyType::CROWBAR);
  doorGo->AddComponent(door);
  objects.emplace_back(doorGo);

  doorGo = new GameObject(8);
  door = new Door(*doorGo, Helpers::Direction::UP, {96, 704}, false, false,
                  Helpers::KeyType::NOKEY);
  doorGo->AddComponent(door);
  objects.emplace_back(doorGo);
  // begin first floor
}

void RoomState::LoadStairs() {
  // begin first floor
  auto stairsGo = new GameObject(8);
  auto stairs = new Stairs(*stairsGo, Helpers::Direction::DOWN, {202, 125},
                           Helpers::Floor::GROUND_FLOOR);
  stairsGo->AddComponent(stairs);
  objects.emplace_back(stairsGo);

  stairsGo = new GameObject(8);
  stairs = new Stairs(*stairsGo, Helpers::Direction::DOWN, {330, 125},
                      Helpers::Floor::GROUND_FLOOR);
  stairsGo->AddComponent(stairs);
  objects.emplace_back(stairsGo);
  // end first floor

  // begin second floor
  stairsGo = new GameObject(8);
  stairs = new Stairs(*stairsGo, Helpers::Direction::UP, {202, 585},
                      Helpers::Floor::FIRST_FLOOR);
  stairsGo->AddComponent(stairs);
  objects.emplace_back(stairsGo);

  stairsGo = new GameObject(8);
  stairs = new Stairs(*stairsGo, Helpers::Direction::UP, {330, 585},
                      Helpers::Floor::FIRST_FLOOR);
  stairsGo->AddComponent(stairs);
  objects.emplace_back(stairsGo);
  // end second floor
}

void RoomState::LoadAntagonist() {
  std::vector<Vec2> path;

  switch (GameData::hope_is_in) {
    case Helpers::Floor::BASEMENT:
      path = {{215, 1092}};
      break;
    case Helpers::Floor::GROUND_FLOOR:
      path = {{263, 297}, {361, 297}, {351, 394}, {89, 383},
              {203, 296}, {268, 202}, {253, 93},  {82, 118},
              {82, 243},  {40, 76},   {197, 80},  {276, 202}};
      break;
    case Helpers::Floor::FIRST_FLOOR:
      path = {{306, 882}, {235, 818}, {344, 724}, {245, 698}, {301, 589},
              {170, 598}, {64, 611},  {166, 801}, {74, 826},  {168, 904},
              {176, 704}, {201, 574}, {304, 688}, {244, 740}, {360, 841}};
      break;
    default:
      path = {{263, 297}};
      break;
  }

  auto ant_cpt = ant->GetComponent(AntagonistType);
  if (!ant_cpt) {
    throw std::runtime_error("sem antagonista em RoomState::LoadAntagonist");
  }
  auto antagonista = std::dynamic_pointer_cast<Antagonist>(ant_cpt);
  antagonista->NewPatrolPath(path);
}

void RoomState::LoadItems() {
  // auto itemGo = new GameObject(7);
  // auto item =
  //     new Item(*itemGo, "Chave 1", "Uma chave com aparência de velha.",
  //              "assets/img/item/key1.png", {256, 177},
  //              Helpers::KeyType::KEY1);
  // itemGo->AddComponent(item);
  // objects.emplace_back(itemGo);

  auto itemGo = new GameObject(7);
  auto item = new Item(
      *itemGo, "Pé de Cabra",
      "Bom para abrir coisas que normalmente não podem ser abertas.",
      "assets/img/item/crowbar.png", {34, 225}, Helpers::KeyType::CROWBAR);
  itemGo->AddComponent(item);
  objects.emplace_back(itemGo);
}
