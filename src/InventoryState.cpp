#include "InventoryState.hpp"
#include "GameData.hpp"
#include "InputManager.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
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
  gridLimit = gridWidth * gridHeight;
  initialPosition = {461, 234};

  menuLength = 2;
  menuItemPositions = {{200, 630}, {820, 630}};
  if ((unsigned)menuLength != menuItemPositions.size()) {
    throw std::runtime_error(
        "Mano, tu acabou de setar o tamanho de um menu e nao colocou o mesmo "
        "tanto de posiçoes, ta de sacanagem ne");
  }

  int index = 0;
  bool showcaseCreated = false;
  for (auto item : GameData::PlayerInventory) {
    if (!showcaseCreated) {
      createShowcase(item);
      showcaseCreated = true;
    }

    auto spriteComponent = item->GetComponent(SpriteType);
    if (!spriteComponent) {
      throw std::runtime_error("Item sem sprite!!");
    }

    auto sprite = std::dynamic_pointer_cast<Sprite>(spriteComponent);
    sprite->SetDimensions(itemWidth - 20, itemHeight - 20);
    auto position = getGridPosition(index % gridLimit);
    item->box.SetCenter(position);
    item->ReverseDelete();
    item->DisableRender();
    objects.push_back(item);
    index += 1;
  }

  cursorGo = std::make_shared<GameObject>(5000);
  Sprite* cursor_sprite = new Sprite(*cursorGo, "assets/img/menu/cursor.png");
  cursorGo->AddComponent(cursor_sprite);
  objects.push_back(cursorGo);

  pageGo = std::make_shared<GameObject>(0);
  Sprite* page_sprite = new Sprite(*pageGo);
  pageGo->AddComponent(page_sprite);
  objects.push_back(pageGo);
  refreshPage();
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
      if (cursorColumn == 0) {
        if (cursorRow == gridHeight - 1) {
          cursorIndex = Page;
        }
      } else {
        cursorColumn -= 1;
      }
    } else if (im.KeyPress(RIGHT_ARROW_KEY)) {
      cursorColumn = std::min(gridWidth - 1, cursorColumn + 1);
    }
    auto newIndex = cursorRow * gridWidth + cursorColumn;
    if (newIndex != inventoryCursorIndex) {
      inventoryCursorIndex = newIndex;
      updateShowcase();
    }
  } else if (cursorIndex == Menu) {
    Vec2 newPos = menuItemPositions[menuCursorIndex];
    cursorGo->box.x = newPos.x;
    cursorGo->box.y = newPos.y;

    if (im.KeyPress(UP_ARROW_KEY)) {
      if (menuCursorIndex == 0) {
        cursorIndex = Page;
      } else if (menuCursorIndex == 1) {
        cursorIndex = Inventory;
        inventoryCursorIndex = (gridHeight - 1) * gridWidth;
        inventoryCursorIndex += gridWidth - 1;
      }
      updateShowcase();
    } else if (im.KeyPress(LEFT_ARROW_KEY)) {
      menuCursorIndex = std::max(0, menuCursorIndex - 1);
    } else if (im.KeyPress(RIGHT_ARROW_KEY)) {
      menuCursorIndex = std::min(menuLength - 1, menuCursorIndex + 1);
    } else if (im.KeyPress(SPACE_BAR_KEY)) {
      switch (menuCursorIndex) {
        case 0:
          popRequested = true;
          break;
        case 1:
          quitRequested = true;
          break;
        default:
          break;
      }
    }
  } else if (cursorIndex == Page) {
    cursorGo->box.x = 100;
    cursorGo->box.y = 550;
    if (im.KeyPress(SPACE_BAR_KEY)) {
      GameData::InventoryPage = (GameData::InventoryPage + 1) % 2;
      refreshPage();
      updateShowcase();
    } else if (im.KeyPress(RIGHT_ARROW_KEY)) {
      cursorIndex = Inventory;
      inventoryCursorIndex = (gridHeight - 1) * gridWidth;
      updateShowcase();
    } else if (im.KeyPress(DOWN_ARROW_KEY)) {
      cursorIndex = Menu;
      menuCursorIndex = 0;
    }
  }

  UpdateArray(dt);
}

void InventoryState::Start() {
  LoadAssets();
  SortObjects();
  StartArray();
  started = true;
}

void InventoryState::refreshPage() {
  int page = GameData::InventoryPage;

  auto spriteCpt = pageGo->GetComponent(SpriteType);
  if (!spriteCpt) {
    throw std::runtime_error("Page sem sprite");
  }
  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  auto sprite_path = (page == 0) ? "assets/img/inventory/items1.png"
                                 : "assets/img/inventory/items2.png";
  sprite->Open(sprite_path);

  int begin = page * gridLimit;
  int end = (page + 1) * gridLimit;
  for (uint32_t i = 0; i < GameData::PlayerInventory.size(); i++) {
    if (i >= begin && i < end) {
      GameData::PlayerInventory[i]->EnableRender();
    } else {
      GameData::PlayerInventory[i]->DisableRender();
    }
  }

  pageGo->box.SetCenter(510, 390);
  inventoryCursorIndex = 0;
}

void InventoryState::createShowcase(std::shared_ptr<GameObject> item) {
  if (showcaseGo) {
    throw std::runtime_error("Tentando criar showcase ja criado!");
  }

  auto spriteComponent = item->GetComponent(SpriteType);
  auto itemComponent = item->GetComponent(ItemType);
  if (!spriteComponent || !itemComponent) {
    throw std::runtime_error(
        "Item sem item ou sem sprite na criacao do showcase!!");
  }

  auto item_item = std::dynamic_pointer_cast<Item>(itemComponent);
  auto item_sprite = std::dynamic_pointer_cast<Sprite>(spriteComponent);

  showcaseTitleGo = std::make_shared<GameObject>(1);
  Text* text =
      new Text(*showcaseTitleGo, "assets/font/tox-typewriter.ttf", 28,
               Text::BLENDED_WRAPPED, item_item->GetName(), {0, 0, 0, 0}, 210);
  showcaseTitleGo->AddComponent(text);
  showcaseTitleGo->box.x = 221;
  showcaseTitleGo->box.y = 240;
  objects.push_back(showcaseTitleGo);

  showcaseDescriptionGo = std::make_shared<GameObject>(1);
  text = new Text(*showcaseDescriptionGo, "assets/font/tox-typewriter.ttf", 25,
                  Text::BLENDED_WRAPPED, item_item->GetDescription(),
                  {0, 0, 0, 0}, 210);
  showcaseDescriptionGo->AddComponent(text);
  showcaseDescriptionGo->box.x = 221;
  showcaseDescriptionGo->box.y = 280;
  objects.push_back(showcaseDescriptionGo);

  showcaseGo = std::make_shared<GameObject>(1);
  Sprite* showcaseSprite = new Sprite(*showcaseGo, item_sprite->GetFile());
  showcaseGo->AddComponent(showcaseSprite);
  showcaseSprite->SetDimensions(120, 270);
  showcaseGo->box.SetCenter({115, 400});
  objects.push_back(showcaseGo);

  SortObjects();
}

void InventoryState::updateShowcase() {
  int itemIndex = inventoryCursorIndex + (GameData::InventoryPage * gridLimit);

  if (itemIndex >= (int)GameData::PlayerInventory.size()) {
    return;
  }

  if (!showcaseGo || !showcaseTitleGo || !showcaseDescriptionGo) {
    throw std::runtime_error("mermao, tu ainda nao criou o showcase");
  }

  auto item = GameData::PlayerInventory[itemIndex];
  auto spriteComponent = item->GetComponent(SpriteType);
  auto itemComponent = item->GetComponent(ItemType);
  if (!spriteComponent || !itemComponent) {
    printf("osh\n");
    throw std::runtime_error("Item sem sprite ou item!!");
  }

  auto item_sprite = std::dynamic_pointer_cast<Sprite>(spriteComponent);
  auto item_item = std::dynamic_pointer_cast<Item>(itemComponent);

  auto spriteCpt = showcaseGo->GetComponent(SpriteType);
  auto titleCpt = showcaseTitleGo->GetComponent(TextType);
  auto descriptionCpt = showcaseDescriptionGo->GetComponent(TextType);
  if (!spriteCpt || !titleCpt || !descriptionCpt) {
    throw std::runtime_error(
        "Sem sprite ou title ou description no showcase oh no");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  auto title = std::dynamic_pointer_cast<Text>(titleCpt);
  auto description = std::dynamic_pointer_cast<Text>(descriptionCpt);

  title->SetText(item_item->GetName());
  description->SetText(item_item->GetDescription());

  sprite->Open(item_sprite->GetFile());
  sprite->SetDimensions(120, 270);
  showcaseGo->box.SetCenter({115, 400});
}

void InventoryState::Pause() {}

void InventoryState::Resume() {}

void InventoryState::LoadAssets() {}

void InventoryState::Render() { RenderArray(); }
