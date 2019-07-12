#ifndef ANTAGONIST_H
#define ANTAGONIST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define BREATHING_ANTAGONIST_SOUND "assets/audio/monster/monster_grunt.wav"
#define ANTAGONIST_WALKING_SOUND "assets/audio/monster/monster_walking.wav"
#define ATTACK_ROAR_ANTAGONIST "assets/audio/monster/monster_scream.wav"

#define LEFT_WALK_SPRITE "assets/img/terry/walk/left_walk.png"
#define RIGHT_WALK_SPRITE "assets/img/terry/walk/right_walk.png"
#define DOWN_WALK_SPRITE "assets/img/terry/walk/down_walk.png"
#define UP_WALK_SPRITE "assets/img/terry/walk/up_walk.png"
#define LEFT_DOWN_WALK_SPRITE "assets/img/terry/walk/down_walk.png"
#define LEFT_UP_WALK_SPRITE "assets/img/terry/walk/up_walk.png"
#define RIGHT_DOWN_WALK_SPRITE "assets/img/terry/walk/down_walk.png"
#define RIGHT_UP_WALK_SPRITE "assets/img/terry/walk/up_walk.png"
#define fdskaljfds "assets/img/terry/idle_terry.png"

#define SUSPECT_LEFT_SPRITE "assets/img/terry/suspect/suspect_left.png"
#define SUSPECT_RIGHT_SPRITE "assets/img/terry/suspect/suspect_right.png"
#define SUSPECT_DOWN_SPRITE "assets/img/terry/suspect/suspect_down.png"
#define SUSPECT_UP_SPRITE "assets/img/terry/suspect/suspect_up.png"

#define PURSUIT_LEFT_SPRITE "assets/img/terry/chase/pursuit_left.png"
#define PURSUIT_RIGHT_SPRITE "assets/img/terry/chase/pursuit_right.png"
#define PURSUIT_DOWN_SPRITE "assets/img/terry/chase/pursuit_down.png"
#define PURSUIT_UP_SPRITE "assets/img/terry/chase/pursuit_up.png"

#define RIGHT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_right.png"
#define LEFT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_left.png"
#define UP_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_down.png"
#define DOWN_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_down.png"
#define UPRIGHT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_down.png"
#define UPLEFT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_down.png"
#define DOWNRIGHT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_down.png"
#define DOWNLEFT_IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle/idle_down.png"

#define RIGHT_ATTACK_SPRITE_ANTAGONIST \
  "assets/img/terry/attack/right_attack.png"
#define LEFT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/attack/left_attack.png"
#define UP_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/attack/up_attack.png"
#define DOWN_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/attack/down_attack.png"
#define UPRIGHT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/attack/up_attack.png"
#define UPLEFT_ATTACK_SPRITE_ANTAGONIST "assets/img/terry/attack/up_attack.png"
#define DOWNRIGHT_ATTACK_SPRITE_ANTAGONIST \
  "assets/img/terry/attack/down_attack.png"
#define DOWNLEFT_ATTACK_SPRITE_ANTAGONIST \
  "assets/img/terry/attack/down_attack.png"

#define CALC_CONST 6
#define PATROL_SPEED 30.0f
#define SUSPECT_SPEED 40.0f
#define PURSUIT_SPEED 70.0f
#define ZERO_SPEED 0

#define WALKING_WALK_SET                                                    \
  {                                                                         \
    LEFT_WALK_SPRITE, RIGHT_WALK_SPRITE, DOWN_WALK_SPRITE, UP_WALK_SPRITE,  \
        LEFT_DOWN_WALK_SPRITE, LEFT_UP_WALK_SPRITE, RIGHT_DOWN_WALK_SPRITE, \
        RIGHT_UP_WALK_SPRITE                                                \
  }

#define SUSPECTING_WALK_SET                                                 \
  {                                                                         \
    SUSPECT_LEFT_SPRITE, SUSPECT_RIGHT_SPRITE, SUSPECT_DOWN_SPRITE, SUSPECT_UP_SPRITE,  \
        SUSPECT_DOWN_SPRITE, SUSPECT_UP_SPRITE, SUSPECT_DOWN_SPRITE, \
        SUSPECT_UP_SPRITE                                                 \
  }

#define CHASING_WALK_SET                                                    \
  {                                                                         \
    PURSUIT_LEFT_SPRITE, PURSUIT_RIGHT_SPRITE, PURSUIT_DOWN_SPRITE, PURSUIT_UP_SPRITE,  \
        PURSUIT_DOWN_SPRITE, PURSUIT_UP_SPRITE, PURSUIT_DOWN_SPRITE, \
        PURSUIT_UP_SPRITE                                                \
  }

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
  Antagonist(GameObject& associated, std::vector<Vec2> path);

  ~Antagonist();

  void Start() override;

  void NotifyCollision(std::shared_ptr<GameObject> other) override;

  void Update(float dt) override;

  bool Is(Types type) const override;

  void Render() override;

  void Push(IFSM* s);

  bool NearTarget();

  void AssetsManager(Helpers::Action action);

  void NewPatrolPath(std::vector<Vec2> path);

  const Types Type = Types::AntagonistType;

  Vec2 position;

  static std::stack<std::pair<unsigned, std::vector<Vec2>>> paths;

 private:
  void MoveAssetsManager(std::vector<std::string> set, float frame_time,
                         bool ac);

  void IdleAssetsManager(float frame_time, bool action_change);

  void AttackAssetsManager(float frame_time);

  Vec2 previous_position;

  IFSM* stored_state;

  std::stack<std::unique_ptr<IFSM>> state_stack;

  std::vector<Vec2> path;

  Helpers::Direction last_direction;

  Helpers::Action last_action;

  std::shared_ptr<GameObject> priorityChanger_go;
};

#endif
