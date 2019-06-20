#ifndef PLAYER_H
#define PLAYER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Sprite.hpp"

class Player : public Component {
 public:
  Player(GameObject& associated, Vec2 position);

  ~Player();

  void Start() override;

  void NotifyCollision(GameObject& other) override;

  void Update(float dt) override;

  bool Is(GameData::Types type) const override;

  void Render() override;

  const GameData::Types Type = GameData::Types::Player;

 private:
  void OpenIdleSprite(const std::shared_ptr<Sprite>& sprite,
                      Helpers::Direction lastDirection);

  bool OpenWalkingSprite(const std::shared_ptr<Sprite>& sprite,
                         Helpers::Direction lastDirection,
                         Helpers::Direction direction);

  Vec2 position;

  Helpers::Direction lastDirection;
};

#endif
