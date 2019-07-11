#ifndef PRIORITYCHANGER_H
#define PRIORITYCHANGER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Rect.hpp"
#include "Types.hpp"

enum PCType {
  PCNormal,
  PCPlayer,
  PCMonster,
};

class PriorityChanger : public Component {
 public:
  PriorityChanger(GameObject& associated, GameObject& object,
                  PCType type = PCType::PCNormal);

  ~PriorityChanger();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void SetRect(float dt, Rect rect);

  const Types Type = PriorityChangerType;

 private:
  struct GameObjectComp {
    bool operator()(const GameObject* lhs, const GameObject* rhs) const {
      return (lhs->box.y + lhs->box.h) < (rhs->box.y + rhs->box.h);
    }
  };

  GameObject& object;

  std::vector<GameObject*> currentCollision;

  PCType type;
};

#endif
