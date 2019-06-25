#ifndef ANTAGONIST_H
#define ANTAGONIST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define ANTAGONIST_SPRITE "assets/img/terry/idle_terry.png"
#define ANTAGONIST_DISTANCE 50

#include <memory>
#include <stack>
#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Timer.hpp"
#include "Types.hpp"

class Antagonist : public Component {
 public:
  Antagonist(GameObject& associated, Vec2 position);

  ~Antagonist();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void Push(IFSM* s);

  bool NearTarget();

  const Types Type = Types::AntagonistType;

  Vec2 position;

 private:
  IFSM* stored_state;

  std::stack<std::unique_ptr<IFSM>> state_stack;
};

#endif
