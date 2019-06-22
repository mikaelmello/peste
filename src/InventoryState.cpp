#include "InventoryState.hpp"
#include "InputManager.hpp"

InventoryState::InventoryState() {
  GameObject* background_go = new GameObject();
  Sprite* background_sprite =
      new Sprite(*background_go, "assets/img/temp_inventory.jpg");
  background_go->AddComponent(background_sprite);
  objects.emplace(background_go);
}

InventoryState::~InventoryState() {}

void InventoryState::Update(float dt) {
  if (quitRequested || popRequested) {
    return;
  }

  InputManager& im = InputManager::GetInstance();
  quitRequested |= im.QuitRequested();
  popRequested |= im.KeyPress(ESCAPE_KEY);

  UpdateArray(dt);
}

void InventoryState::Start() {
  LoadAssets();
  StartArray();
  started = true;
}

void InventoryState::Pause() {}

void InventoryState::Resume() {}

void InventoryState::LoadAssets() {}

void InventoryState::Render() { RenderArray(); }
