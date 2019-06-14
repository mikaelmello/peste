#ifndef PATROLSTATE_H
#define PATROLSTATE_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stack>
#include "GameObject.hpp"
#include "IState.hpp"
#include "Timer.hpp"

class PatrolState : public IState {
 public:
  PatrolState(GameObject& antagonist);

  ~PatrolState();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

  friend class IState;

 private:
  int counter;

  Timer timer;

  std::stack<std::vector<Vec2>> patrol_paths;
};

#endif
