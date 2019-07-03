#include "InventoryState.hpp"
#include "GameData.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

InventoryState::InventoryState()
    : inventoryCursorIndex(0), cursorIndex(Inventory) {
  GameObject* background_go = new GameObject(-1);
  Sprite* background_sprite =
      new Sprite(*background_go, "assets/img/inventory/background.png");
  background_go->AddComponent(background_sprite);
  objects.emplace_back(background_go);

  // based on the image loaded above
  itemWidth = 158;
  itemHeight = 105;
  gridWidth = 3;
  gridHeight = 3;
  initialPosition = {461, 234};

  menuLength = 3;
  menuItemPositions = {{200, 700}, {511, 700}, {820, 700}};
  if (menuLength != menuItemPositions.size()) {
    throw new std::runtime_error(
        "Mano, tu acabou de setar o tamanho de um menu e nao colocou o mesmo "
        "tanto de posiçoes, ta de sacanagem ne");
  }

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
    objects.push_back(item);
    index += 1;
  }

  cursorGo = std::make_shared<GameObject>(5);
  Sprite* cursor_sprite = new Sprite(*cursorGo, "assets/img/menu/cursor.png");
  cursorGo->AddComponent(cursor_sprite);
  objects.push_back(cursorGo);
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

  if (cursorIndex == Inventory) {
    auto cursorPos = getGridPosition(inventoryCursorIndex);
    cursorGo->box.x = cursorPos.x;
    cursorGo->box.y = cursorPos.y;

    int cursorRow = inventoryCursorIndex / gridWidth;
    int cursorColumn = (inventoryCursorIndex % gridWidth);
    if (im.KeyPress(UP_ARROW_KEY)) {
      cursorRow = std::max(0, cursorRow - 1);
    } else if (im.KeyPress(DOWN_ARROW_KEY)) {
      if (cursorRow == gridHeight - 1) {
        cursorIndex = Menu;
        menuCursorIndex = menuLength - 1;
        if (cursorColumn == 0) {
          menuCursorIndex--;
        }
      }
      cursorRow = std::min(gridHeight - 1, cursorRow + 1);
    } else if (im.KeyPress(LEFT_ARROW_KEY)) {
      cursorColumn = std::max(0, cursorColumn - 1);
    } else if (im.KeyPress(RIGHT_ARROW_KEY)) {
      cursorColumn = std::min(gridWidth - 1, cursorColumn + 1);
    }
    inventoryCursorIndex = cursorRow * gridWidth + cursorColumn;
  } else if (cursorIndex == Menu) {
    Vec2 newPos = menuItemPositions[menuCursorIndex];
    cursorGo->box.x = newPos.x;
    cursorGo->box.y = newPos.y;

    if (im.KeyPress(UP_ARROW_KEY)) {
      cursorIndex = Inventory;
      inventoryCursorIndex = (gridHeight - 1) * gridWidth;
      if (menuCursorIndex == 2) {
        inventoryCursorIndex += gridWidth - 1;
      }
    } else if (im.KeyPress(LEFT_ARROW_KEY)) {
      menuCursorIndex = std::max(0, menuCursorIndex - 1);
    } else if (im.KeyPress(RIGHT_ARROW_KEY)) {
      menuCursorIndex = std::min(menuLength - 1, menuCursorIndex + 1);
    }
  }

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
