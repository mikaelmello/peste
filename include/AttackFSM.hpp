#ifndef ATTACKFSM_H
#define ATTACKFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Timer.hpp"

class AttackFSM : public IFSM {
 public:
  AttackFSM(GameObject& object);

  ~AttackFSM();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

 private:
  float execution_time;

  Timer timer;
};

#endif