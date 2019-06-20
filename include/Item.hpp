#ifndef ITEM_H
#define ITEM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Vec2.hpp"

class Item : public Component {
 public:
  Item(GameObject& associated, const std::string& name,
       const std::string& description, const std::string& spritePath, Vec2 pos);

  ~Item();

  void Start() override;

  void NotifyCollision(GameObject& other) override;

  void Update(float dt) override;

  bool Is(GameData::Types type) const override;

  void Render() override;

  void SetCenter(Vec2 pos);

  void SetScale(float scaleX, float scaleY);

  const GameData::Types Type = GameData::Types::Item;

 private:
  std::string name;
  std::string description;
  Vec2 position;
};

#endif
