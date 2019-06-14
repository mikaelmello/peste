#ifndef ISTATE_H
#define ISTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GameObject.hpp"

class IState {
 public:
  IState(GameObject& antagonist);

  ~IState();

  void virtual OnStateEnter() = 0;

  void virtual OnStateExecution() = 0;

  void virtual OnStateExit() = 0;

  void virtual Update(float dt) = 0;

 protected:
  GameObject& antagonist;
};

#endif
