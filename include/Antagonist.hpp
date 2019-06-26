#ifndef ANTAGONIST_H
#define ANTAGONIST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define LEFT_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define LEFT_DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define LEFT_UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define IDLE_SPRITE "assets/img/terry/idle_terry.png"
#define ANTAGONIST_DISTANCE 50

#include <memory>
#include <stack>
#include <string>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
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

  void SpriteManager(Helpers::Action action);

  const Types Type = Types::AntagonistType;

  Vec2 position;

 private:
  void SpriteMovement();

  Vec2 previous_position;

  IFSM* stored_state;

  std::stack<std::unique_ptr<IFSM>> state_stack;

  Helpers::Direction last_direction;
};

#endif
