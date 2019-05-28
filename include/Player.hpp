#ifndef PLAYER_H
#define PLAYER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"

class Player : public Component {
 public:
  Player(GameObject& associated);

  ~Player();

  void Start() override;

  void NotifyCollision(GameObject& other) override;

  void Update(float dt) override;

  bool Is(GameData::Types type) const override;

  void Render() override;

  const GameData::Types Type = GameData::Types::Player;

 private:
};

#endif
