#ifndef IDLEFSM_H
#define IDLEFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define IDLE_TIME 2
#define IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"

#include <stack>
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Timer.hpp"

class IdleFSM : public IFSM {
 public:
  IdleFSM(GameObject& antagonist);

  ~IdleFSM();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

  friend class IState;

 private:
  int counter;

  Timer timer;
};

#endif