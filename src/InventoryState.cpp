#include "InventoryState.hpp"
#include "GameData.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

InventoryState::InventoryState() : cursorPositionIndex(0) {
  GameObject* background_go = new GameObject(-1);
  Sprite* background_sprite =
      new Sprite(*background_go, "assets/img/inventory/background.png");
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
    objects.insert(item);
    index += 1;
  }

  cursorGo = std::make_shared<GameObject>(5);
  Sprite* cursor_sprite = new Sprite(*cursorGo, "assets/img/menu/cursor.png");
  cursorGo->AddComponent(cursor_sprite);
  objects.insert(cursorGo);
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

  int cursorRow = cursorPositionIndex / gridWidth;
  int cursorColumn = (cursorPositionIndex % gridWidth);

  if (im.KeyPress(UP_ARROW_KEY)) {
    cursorRow = std::max(0, cursorRow - 1);
  } else if (im.KeyPress(DOWN_ARROW_KEY)) {
    cursorRow = std::min(gridHeight - 1, cursorRow + 1);
  } else if (im.KeyPress(LEFT_ARROW_KEY)) {
    cursorColumn = std::max(0, cursorColumn - 1);
  } else if (im.KeyPress(RIGHT_ARROW_KEY)) {
    cursorColumn = std::min(gridWidth - 1, cursorColumn + 1);
  }
  cursorPositionIndex = cursorRow * gridWidth + cursorColumn;
  auto cursorPos = getGridPosition(cursorPositionIndex);
  cursorGo->box.x = cursorPos.x;
  cursorGo->box.y = cursorPos.y;
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
