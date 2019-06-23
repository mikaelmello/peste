#ifndef ITEM_H
#define ITEM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

class Item : public Component {
 public:
  Item(GameObject& associated, const std::string& name,
       const std::string& description, const std::string& spritePath, Vec2 pos);

  ~Item();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void SetCenter(Vec2 pos);

  void SetScale(float scaleX, float scaleY);

  const Types Type = ItemType;

 private:
  std::string name;
  std::string description;
  Vec2 position;
  bool colliding;
};

#endif
