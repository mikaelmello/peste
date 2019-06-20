#ifndef IFSM_H
#define IFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GameObject.hpp"

class IFSM {
 public:
  IFSM(GameObject& object);

  ~IFSM();

  inline bool PopRequested() { return pop_requested; }

  void virtual OnStateEnter() = 0;

  void virtual OnStateExecution() = 0;

  void virtual OnStateExit() = 0;

  void virtual Update(float dt) = 0;

 protected:
  GameObject& antagonist;

  bool pop_requested;
};

#endif
