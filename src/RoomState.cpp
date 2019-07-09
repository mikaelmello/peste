#include "RoomState.hpp"
#include <iostream>
#include "Antagonist.hpp"
#include "Camera.hpp"
#include "CameraFollower.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Door.hpp"
#include "Furniture.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "InventoryState.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "Terry.hpp"
#include "TileMap.hpp"
#include "Types.hpp"

#define IMG_PATH "assets/img/furniture/"

RoomState::RoomState() {
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
  popRequested |= im.KeyPress(ESCAPE_KEY);

  if (im.KeyPress(I_KEY)) {
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
  LoadFurnitureFirstFloor();
  LoadFurnitureSecondFloor();
  LoadFurnitureBasement();
  LoadDoors();

  auto playerGo = std::make_shared<GameObject>(8);
  Player* player = new Player(*playerGo, currentTileMap->GetInitialPosition());
  playerGo->AddComponent(player);
  objects.push_back(playerGo);
  GameData::PlayerGameObject = playerGo;

<<<<<<< HEAD
  std::cout << objects.size() << std::endl;
  // GameObject* terryGo = new GameObject(5);
  // Terry* terry = new Terry(
  //     *terryGo, currentTileMap->GetInitialPosition() + Vec2(-18, -18));
  // terryGo->AddComponent(terry);
  // objects.emplace_back(terryGo);
=======
  GameObject* antagonist_go = new GameObject(5);
  std::vector<Vec2> points = {{68, 199},  {88, 132},  {110, 66},
                              {199, 106}, {332, 132}, {326, 195}};
  Antagonist* antagonist = new Antagonist(*antagonist_go, points);
  antagonist_go->AddComponent(antagonist);
  objects.emplace_back(antagonist_go);
>>>>>>> refact/terry-walk

  // GameObject* antagonist_go = new GameObject(5);
  // Antagonist* antagonist =
  //     new Antagonist(*antagonist_go, currentTileMap->GetInitialPosition());
  // antagonist_go->AddComponent(antagonist);
  // objects.emplace_back(antagonist_go);

  // GameObject* door_go = new GameObject(6);
  // Vec2 door_pos = currentTileMap->GetInitialPosition() + Vec2(8, 8);
  // Door* door = new Door(*door_go, Helpers::Direction::LEFT, door_pos, false);
  // door_go->AddComponent(door);
  // objects.emplace_back(door_go);

  Camera::Follow(playerGo.get());

  // GameObject* lampGo = new GameObject(6);
  // Item* lamp =
  //     new Item(*lampGo, "Lamparina",
  //              "Esta lamparina é a única coisa permitindo que Hope veja "
  //              "ao seu redor e não seja consumido pela escuridão.",
  //              "assets/img/item/lamp.png",
  //              currentTileMap->GetInitialPosition() - Vec2(3, 3));
  // lampGo->AddComponent(lamp);
  // objects.emplace_back(lampGo);

  // GameObject* lamp2Go = new GameObject(6);
  // Item* lamp2 =
  //     new Item(*lamp2Go, "Lamparina 2", "Esta lamparina é ruim pode
  //     esquecer",
  //              "assets/img/item/lamp2.png",
  //              currentTileMap->GetInitialPosition() + Vec2(3, 3));
  // lamp2Go->AddComponent(lamp2);
  // objects.emplace_back(lamp2Go);

  SortObjects();
}

void RoomState::Render() { RenderArray(); }

void RoomState::LoadFurnitureFirstFloor() {
  GameObject* furnitureGo = new GameObject(6);
  Furniture* furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/cabinet.png", {53, 340},
                    Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  // begin piano room
  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/piano.png",
                            {258, 314}, Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/piano_stool.png",
                    {265, 352}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/sofa_front.png",
                            {183, 305}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/sofa_front.png",
                            {324, 305}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/misc_shelf.png",
                            {150, 238}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/armchair.png",
                            {306, 258}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/armchair.png",
                            {359, 258}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end piano room

  // begin kitchen
  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/table_big.png",
                            {305, 64}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/cabinet.png",
                            {257, 36}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/pantry3.png",
                            {66, 174}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/pantry1.png",
                            {99, 174}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/wall_cabinet.png",
                    {32, 15}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end kitchen
}

void RoomState::LoadFurnitureSecondFloor() {
  // begin bedroom 1
  auto furnitureGo = new GameObject(7);
  auto furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/cabinet2.png",
                    {22, 535}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bed2.png",
                            {91, 545}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/bedside_table.png",
                    {73, 555}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/bedside_table.png",
                    {126, 555}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end bedroom1

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/clock.png",
                            {210, 530}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  // begin library
  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/armchair.png",
                            {231, 660}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/desk.png",
                            {346, 652}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/book_shelf3.png",
                    {367, 757}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/book_shelf2.png",
                    {334, 757}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/book_shelf1.png",
                    {206, 757}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/table.png",
                            {233, 855}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end library

  // begin bedroom terry
  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/wardrobe.png",
                            {118, 719}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/shelf.png",
                            {103, 723}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bedTerry.png",
                            {55, 736}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/toy2.png",
                            {37, 817}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/toy3.png",
                            {52, 807}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end bedroom terry

  // begin last bedroom
  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/cabinet.png",
                            {35, 870}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bedgreen.png",
                            {69, 867}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/bedside_table.png",
                    {90, 873}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end last bedroom
}

void RoomState::LoadFurnitureBasement() {
  // begin first room
  auto furnitureGo = new GameObject(7);
  auto furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/wheelchair.png",
                    {178, 1059}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bedPacient.png",
                            {254, 1059}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end first room

  // begin second room
  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bedPacient.png",
                            {300, 1059}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/serum.png",
                            {290, 1052}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableRight.png",
                    {384, 1063}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/microscope2.png",
                    {386, 1063}, Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end second room

  // begin lab
  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableFront.png",
                    {47, 1145}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/lab_stuff.png",
                            {46, 1146}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/equipament.png",
                            {60, 1144}, Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/microscope2.png",
                    {74, 1144}, Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableFront.png",
                    {107, 1145}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/oldpc.png",
                            {123, 1141}, Interaction::LOOK);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/bookpile.png",
                            {110, 1145}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableLeft.png",
                    {17, 1170}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/microscope1.png",
                    {19, 1174}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/papers.png",
                            {19, 1190}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/chairSteel.png",
                            {174, 1160}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/chairSteel.png",
                            {225, 1221}, Interaction::NONE, false);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(6);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableRight.png",
                    {240, 1217}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture =
      new Furniture(*furnitureGo, "assets/img/furniture/labTableRight.png",
                    {240, 1242}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);

  furnitureGo = new GameObject(7);
  furniture = new Furniture(*furnitureGo, "assets/img/furniture/openBook.png",
                            {242, 1221}, Interaction::NONE);
  furnitureGo->AddComponent(furniture);
  objects.emplace_back(furnitureGo);
  // end lab
}

void RoomState::LoadDoors() {
  auto doorGo = new GameObject(7);
  auto door =
      new Door(*doorGo, Helpers::Direction::UP, {256, 229}, false, true);
  doorGo->AddComponent(door);
  objects.emplace_back(doorGo);
}