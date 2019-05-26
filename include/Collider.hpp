#ifndef COLLIDER_H
#define COLLIDER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"

class Collider : public Component {
 public:
  Collider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});

  void Update(float dt) override;

  bool Is(GameData::Types type) const override;

  void Render() override;

  void SetScale(Vec2 scale);

  void SetOffset(Vec2 offset);

  Rect box;

  const GameData::Types Type = GameData::Types::Collider;

 private:
  Vec2 scale;
  Vec2 offset;
};

#endif
