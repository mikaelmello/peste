#ifndef INVENTORYSTATE_H
#define INVENTORYSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Music.hpp"
#include "State.hpp"

class InventoryState : public State {
 public:
  InventoryState();

  ~InventoryState();

  void Start();

  void Pause();

  void Resume();

  bool QuitRequested() const;

  void LoadAssets();

  void Update(float dt);

  void Render();

 private:
  // Width of the inventory item
  int itemWidth;

  // Height of the inventory item
  int itemHeight;

  // Height of the inventory grid
  int gridWidth;

  // Height of the inventory grid
  int gridHeight;

  // Length of the menu row
  int menuLength;

  std::vector<Vec2> menuItemPositions;

  // position of the first item in the grid
  Vec2 initialPosition;

  // Gets the XY position to render an item
  Vec2 getGridPosition(int index);

  std::shared_ptr<GameObject> cursorGo;

  // Index of the inventory item the cursor is on
  int inventoryCursorIndex;

  // Index of the menu item the cursor is on
  int menuCursorIndex;

  enum CursorPosition {
    Inventory,
    Menu,
  };

  CursorPosition cursorIndex;
};

#endif
