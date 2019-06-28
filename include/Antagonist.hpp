#ifndef ANTAGONIST_H
#define ANTAGONIST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define BREATHING_ANTAGONIST_SOUND "assets/audio/terry/monster_breathing.wav"

#define LEFT_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define LEFT_DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define LEFT_UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define IDLE_SPRITE "assets/img/terry/idle_terry.png"

#define RIGHT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define LEFT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define UP_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define DOWN_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define UPRIGHT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define UPLEFT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define DOWNRIGHT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define DOWNLEFT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"

#define RIGHT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define LEFT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define UP_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define DOWN_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define UPRIGHT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define UPLEFT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define DOWNRIGHT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"
#define DOWNLEFT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"

#define WALKING_WALK_SET                                                    \
  {                                                                         \
    LEFT_WALK_SPRITE, RIGHT_WALK_SPRITE, DOWN_WALK_SPRITE, UP_WALK_SPRITE,  \
        LEFT_DOWN_WALK_SPRITE, LEFT_UP_WALK_SPRITE, RIGHT_DOWN_WALK_SPRITE, \
        RIGHT_UP_WALK_SPRITE                                                \
  }

#define SUSPECTING_WALK_SET                                                 \
  {                                                                         \
    LEFT_WALK_SPRITE, RIGHT_WALK_SPRITE, DOWN_WALK_SPRITE, UP_WALK_SPRITE,  \
        LEFT_DOWN_WALK_SPRITE, LEFT_UP_WALK_SPRITE, RIGHT_DOWN_WALK_SPRITE, \
        RIGHT_UP_WALK_SPRITE                                                \
  }

#define CHASING_WALK_SET                                                    \
  {                                                                         \
    LEFT_WALK_SPRITE, RIGHT_WALK_SPRITE, DOWN_WALK_SPRITE, UP_WALK_SPRITE,  \
        LEFT_DOWN_WALK_SPRITE, LEFT_UP_WALK_SPRITE, RIGHT_DOWN_WALK_SPRITE, \
        RIGHT_UP_WALK_SPRITE                                                \
  }

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

  void AssetsManager(Helpers::Action action);

  const Types Type = Types::AntagonistType;

  Vec2 position;

 private:
  void MoveAssetsManager(std::vector<std::string> set);

  void IdleAssetsManager();

  void AttackAssetsManager();

  Vec2 previous_position;

  IFSM* stored_state;

  std::stack<std::unique_ptr<IFSM>> state_stack;

  Helpers::Direction last_direction;

  Helpers::Action last_action;
};

#endif
