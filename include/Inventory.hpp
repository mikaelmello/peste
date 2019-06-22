#ifndef INVENTORY_H
#define INVENTORY_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Types.hpp"

class Inventory : public Component {
 public:
  Inventory(GameObject& associated);

  ~Inventory();

  void Start() override;

  void NotifyCollision(GameObject& other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  const Types Type = InventoryType;

 private:
};

#endif
