#ifndef BLOCKER_H
#define BLOCKER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "TileMap.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

class Blocker : public Component {
 public:
  Blocker(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});

  ~Blocker();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  const Types Type = BlockerType;

  void Block();

  void Unblock();

  bool IsBlocking() const;

 private:
  bool block;

  bool colliding;

  int beginX, endX, beginY, endY;

  TileMap* tilemap;
};

#endif
