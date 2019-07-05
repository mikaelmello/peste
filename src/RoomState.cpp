#include "RoomState.hpp"
#include <iostream>
#include "Antagonist.hpp"
#include "Camera.hpp"
#include "CameraFollower.hpp"
#include "Collider.hpp"
#include "Collision.hpp"
#include "Door.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "InventoryState.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "TileMap.hpp"
#include "Types.hpp"

RoomState::RoomState() {
  GameObject* bckgGo = new GameObject(5);
  Sprite* bckgSprite = new Sprite(*bckgGo, "assets/img/black.jpg");
  bckgGo->AddComponent(new CameraFollower(*bckgGo, {512, 334}));
  bckgGo->AddComponent(bckgSprite);
  objects.emplace_back(bckgGo);

  GameObject* mapGo = new GameObject(5);
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
  auto playerGo = std::make_shared<GameObject>(7);
  Player* player = new Player(*playerGo, currentTileMap->GetInitialPosition());
  playerGo->AddComponent(player);
  objects.push_back(playerGo);
  GameData::PlayerGameObject = playerGo;

  // GameObject* antagonist_go = new GameObject(5);
  // Antagonist* antagonist =
  //     new Antagonist(*antagonist_go, currentTileMap->GetInitialPosition());
  // antagonist_go->AddComponent(antagonist);
  // objects.emplace_back(antagonist_go);

  GameObject* door_go = new GameObject(6);
  Vec2 door_pos = currentTileMap->GetInitialPosition() + Vec2(8, 8);
  Door* door = new Door(*door_go, Helpers::Direction::LEFT, door_pos, false);
  door_go->AddComponent(door);
  objects.emplace_back(door_go);

  Camera::Follow(playerGo.get());

  GameObject* lampGo = new GameObject(6);
  Item* lamp =
      new Item(*lampGo, "Lamparina",
               "Esta lamparina é a única coisa permitindo que Hope veja "
               "ao seu redor e não seja consumido pela escuridão.",
               "assets/img/item/lamp.png",
               currentTileMap->GetInitialPosition() - Vec2(3, 3));
  lampGo->AddComponent(lamp);
  objects.emplace_back(lampGo);

  GameObject* lamp2Go = new GameObject(6);
  Item* lamp2 =
      new Item(*lamp2Go, "Lamparina 2", "Esta lamparina é ruim pode esquecer",
               "assets/img/item/lamp2.png",
               currentTileMap->GetInitialPosition() + Vec2(3, 3));
  lamp2Go->AddComponent(lamp2);
  objects.emplace_back(lamp2Go);

  SortObjects();
}

void RoomState::Render() { RenderArray(); }
