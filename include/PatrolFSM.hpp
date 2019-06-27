#ifndef PATROLFSM_H
#define PATROLFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stack>
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "SuspectFSM.hpp"
#include "Timer.hpp"

class PatrolFSM : public IFSM {
 public:
  PatrolFSM(GameObject& object);

  ~PatrolFSM();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

  friend class SuspectFSM;  // Verificar como reduzir o escopo de friend.

 private:
   static std::stack<std::pair<int, std::vector<Vec2>>> patrol_paths;
};

#endif
