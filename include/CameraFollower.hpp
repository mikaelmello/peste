#ifndef CAMERAFOLLOWOER_H
#define CAMERAFOLLOWOER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Vec2.hpp"

class CameraFollower : public Component {
 public:
  CameraFollower(GameObject& associated, Vec2 offset = {0, 0});

  void Update(float dt) override;

  bool Is(GameData::Types type) const override;

  void Render() override;

  const GameData::Types Type = GameData::Types::CameraFollower;

 private:
  Vec2 offset;
};

#endif
