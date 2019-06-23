#ifndef PLAYER_H
#define PLAYER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Sprite.hpp"
#include "Types.hpp"

class Player : public Component {
 public:
  Player(GameObject& associated, Vec2 position);

  ~Player();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  const Types Type = PlayerType;

 private:
  void OpenIdleSprite(const std::shared_ptr<Sprite>& sprite,
                      Helpers::Direction lastDirection);

  void OpenWalkingSprite(const std::shared_ptr<Sprite>& sprite,
                         Helpers::Direction lastDirection,
                         Helpers::Direction direction);

  Vec2 position;

  Helpers::Direction lastDirection;
};

#endif
