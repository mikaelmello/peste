#ifndef PATROLFSM_H
#define PATROLFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <stack>
#include "Antagonist.hpp"
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "SuspectFSM.hpp"
#include "Timer.hpp"

class PatrolFSM : public IFSM {
 public:
  PatrolFSM(GameObject& object, std::vector<Vec2>& points);

  ~PatrolFSM();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

  friend class SuspectFSM;  // Verificar como reduzir o escopo de friend.

 private:
  std::stack<std::pair<unsigned, std::vector<Vec2>>> paths;

  std::weak_ptr<Antagonist> ant;
};

#endif
