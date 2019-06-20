#include "RoomState.hpp"
#include "Camera.hpp"
#include "CameraFollower.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Inventory.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "TileMap.hpp"

RoomState::RoomState() {
  GameObject* bckgGo = new GameObject(5);
  Sprite* bckgSprite = new Sprite(*bckgGo, "assets/img/black.jpg");
  bckgGo->AddComponent(new CameraFollower(*bckgGo, {512, 334}));
  bckgGo->AddComponent(bckgSprite);
  objects.emplace(bckgGo);

  GameObject* mapGo = new GameObject(5);
  currentTileMap = new TileMap(*mapGo, "assets/map/map.json");
  currentTileMap->SetParallax(1, 0, 0);
  mapGo->AddComponent(currentTileMap);
  objects.emplace(mapGo);

  GameObject* playerGo = new GameObject(5);
  Player* player = new Player(*playerGo, currentTileMap->GetInitialPosition());
  playerGo->AddComponent(player);
  objects.emplace(playerGo);

  Camera::Follow(playerGo);

  GameObject* lampGo = new GameObject(5);
  Item* lamp = new Item(*lampGo, "Lamp", "A lamp", "assets/img/item/lamp.png",
                        currentTileMap->GetInitialPosition());
  lampGo->AddComponent(lamp);
  objects.emplace(lampGo);

  GameObject* inventoryGo = new GameObject(1000);
  Inventory* inv = new Inventory(*inventoryGo);
  inventoryGo->AddComponent(inv);
  inventoryGo->DisableRender();
  objects.emplace(inventoryGo);
}

RoomState::~RoomState() {
  Camera::Unfollow();
  Camera::pos = {0, 0};
}

void RoomState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  Camera::Update(dt);

  InputManager& im = InputManager::GetInstance();
  quitRequested |= im.QuitRequested();
  popRequested |= im.KeyPress(ESCAPE_KEY);

  UpdateArray(dt);
}

void RoomState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void RoomState::Pause() {}

void RoomState::Resume() {}

void RoomState::LoadAssets() {}

void RoomState::Render() { RenderArray(); }
