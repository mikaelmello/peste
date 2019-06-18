#ifndef PATROLFSM_H
#define PATROLFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define LEFT_WALK_CODE 0
#define LEFT_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_WALK_CODE 1
#define RIGHT_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define DOWN_WALK_CODE 2
#define DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define UP_WALK_CODE 3
#define UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define LEFT_DOWN_WALK_CODE 4
#define LEFT_DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define LEFT_UP_WALK_CODE 5
#define LEFT_UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_DOWN_WALK_CODE 6
#define RIGHT_DOWN_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define RIGHT_UP_WALK_CODE 7
#define RIGHT_UP_WALK_SPRITE "assets/img/terry/idle_terry.png"
#define IDLE_CODE 8
#define IDLE_SPRITE "assets/img/terry/idle_terry.png"

#include <stack>
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Timer.hpp"

class PatrolFSM : public IFSM {
 public:
  PatrolFSM(GameObject& antagonist);

  ~PatrolFSM();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

  friend class IState;

 private:
  int counter;

  int sprite_status;

  Timer timer;

  std::stack<std::vector<Vec2>> patrol_paths;
};

#endif
