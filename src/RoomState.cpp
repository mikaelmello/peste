#include "RoomState.hpp"
#include "Camera.hpp"
#include "CameraFollower.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "Player.hpp"
#include "TileMap.hpp"

RoomState::RoomState() {
  GameObject* bckgGo = new GameObject(5);
  Sprite* bckgSprite = new Sprite(*bckgGo, "assets/img/black.jpg");
  bckgGo->AddComponent(new CameraFollower(*bckgGo));
  bckgGo->AddComponent(bckgSprite);
  objects.emplace(bckgGo);

  GameObject* mapGo = new GameObject(5);
  TileMap* map = new TileMap(*mapGo, "assets/map/map.json");
  map->SetParallax(1, 0, 0);
  mapGo->AddComponent(map);
  objects.emplace(mapGo);

  GameObject* playerGo = new GameObject(5);
  Player* player = new Player(*playerGo);
  playerGo->AddComponent(player);
  objects.emplace(playerGo);

  Camera::Follow(playerGo);
}

RoomState::~RoomState() {}

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
