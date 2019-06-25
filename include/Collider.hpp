#ifndef COLLIDER_H
#define COLLIDER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Rect.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

class Collider : public Component {
 public:
  Collider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void SetScale(Vec2 scale);

  void SetOffset(Vec2 offset);

  Vec2 GetOffSet() const;

  Vec2 GetScale() const;

  Rect box;

  const Types Type = ColliderType;

 private:
  Vec2 scale;
  Vec2 offset;
};

#endif
