#ifndef PRIORITYCHANGER_H
#define PRIORITYCHANGER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Types.hpp"

class PriorityChanger : public Component {
 public:
  PriorityChanger(GameObject& associated, GameObject& object,
                  bool player = false);

  ~PriorityChanger();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  const Types Type = PriorityChangerType;

 private:
  GameObject& object;

  bool player;
};

#endif
