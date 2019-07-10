#ifndef ITEM_H
#define ITEM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

class Item : public Component {
 public:
  Item(GameObject& associated, const std::string& name,
       const std::string& description, const std::string& spritePath, Vec2 pos,
       Helpers::KeyType keyType = Helpers::KeyType::NOKEY);

  ~Item();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void SetCenter(Vec2 pos);

  void SetScale(float scaleX, float scaleY);

  void HidePickupDialog();

  void ShowPickupDialog();

  void Pickup();

  std::string GetName();

  std::string GetDescription();

  Helpers::KeyType GetKeyType();

  const Types Type = ItemType;

 private:
  std::string name;
  std::string description;
  Vec2 position;
  bool colliding;

  std::shared_ptr<GameObject> pickupItemGo;
  std::shared_ptr<GameObject> priorityChangerGo;

  Helpers::KeyType keyType;
};

#endif
