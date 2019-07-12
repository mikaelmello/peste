#ifndef IDLEFSM_H
#define IDLEFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define IDLE_TIME (0.1)
#define IDLE_SPRITE_ANTAGONIST "assets/img/terry/idle_terry.png"

#include <stack>
#include "Antagonist.hpp"
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Timer.hpp"

class IdleFSM : public IFSM {
 public:
  IdleFSM(GameObject& antagonist);

  ~IdleFSM();

  void OnStateEnter();

  void OnStateExecution(float dt);

  void OnStateExit();

  void Update(float dt);

  friend class IState;

 private:
  int counter;

  Timer timer;

  std::weak_ptr<Antagonist> ant;
};

#endif
