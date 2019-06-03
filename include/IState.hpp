#ifndef ISTATE_H
#define ISTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Antagonist.hpp"

class IState {
 public:
  IState(Antagonist& antagonist);

  ~IState();

  void virtual OnStateEnter() = 0;

  void virtual OnStateExecution() = 0;

  void virtual OnStateExit() = 0;

  void virtual Update(float dt);

 protected:
  Antagonist& antagonist;
};

#endif
