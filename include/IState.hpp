#ifndef ISTATE_H
#define ISTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Antagonist.hpp"
#include "GameObject.hpp"

class IState {
 public:
  IState(GameObject& antagonist);

  ~IState();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

 protected:
  GameObject& antagonist;
};

#endif
