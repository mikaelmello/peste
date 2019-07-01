#include "InventoryState.hpp"
#include "GameData.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

InventoryState::InventoryState() {
  GameObject* background_go = new GameObject(-1);
  Sprite* background_sprite =
      new Sprite(*background_go, "assets/img/temp_inventory.png");
  background_go->AddComponent(background_sprite);
  objects.emplace(background_go);

  // based on the image loaded above
  itemWidth = 158;
  itemHeight = 105;
  gridWidth = 3;
  gridHeight = 3;
  initialPosition = {461, 234};

  int index = 0;
  for (auto item : GameData::PlayerInventory) {
    auto spriteComponent = item->GetComponent(SpriteType);
    if (!spriteComponent) {
      throw std::runtime_error("Item sem sprite!!");
    }

    auto sprite = std::dynamic_pointer_cast<Sprite>(spriteComponent);
    sprite->SetDimensions(itemWidth - 20, itemHeight - 20);
    auto position = getGridPosition(index);
    item->box.SetCenter(position);
    item->ReverseDelete();
    objects.emplace(item);
    index += 1;
  }
}

Vec2 InventoryState::getGridPosition(int index) {
  int row = index / gridWidth;
  int column = (index % gridWidth);

  Vec2 position = initialPosition;

  position.x += column * itemWidth;
  position.y += row * itemHeight;

  return {position.x + itemWidth / 2, position.y + itemHeight / 2};
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
