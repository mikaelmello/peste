#ifndef CAMERAFOLLOWOER_H
#define CAMERAFOLLOWOER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

class CameraFollower : public Component {
 public:
  CameraFollower(GameObject& associated, Vec2 offset = {0, 0});

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  const Types Type = CameraFollowerType;

 private:
  Vec2 offset;
};

#endif
