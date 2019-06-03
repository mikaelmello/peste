#ifndef PATROLSTATE_H
#define PATROLSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "IState.hpp"

class PatrolState : public IState {
 public:
  PatrolState(Antagonist& antagonist);

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);
};

#endif
