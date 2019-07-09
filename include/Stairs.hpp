#ifndef STAIRS_H
#define STAIRS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Types.hpp"

class Stairs : public Component {
 public:
  Stairs(GameObject& associated, Helpers::Direction direction, Vec2 position,
         Helpers::Floor floor);

  ~Stairs();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void ShowInteractDialog();
  void HideInteractDialog();

  const Types Type = StairsType;

 private:
  std::shared_ptr<GameObject> actionMessageGo;
  bool colliding;
};

#endif
