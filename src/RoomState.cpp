#include "RoomState.hpp"
#include "GameObject.hpp"
#include "InputManager.hpp"
#include "TileMap.hpp"

RoomState::RoomState() {
  GameObject* mapGo = new GameObject(5);
  TileMap* map = new TileMap(*mapGo, "assets/map/map.json");
  map->SetParallax(1, 0.5, 0.5);
  mapGo->AddComponent(map);
  objects.emplace(mapGo);
}

RoomState::~RoomState() {}

void RoomState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

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
