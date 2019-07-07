#ifndef ATTACKFSM_H
#define ATTACKFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Antagonist.hpp"
#include "Collider.hpp"
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
  bool IsColliding();

  float execution_time;

  Timer timer;

  std::weak_ptr<Antagonist> ant;

  std::weak_ptr<Collider> ant_col;

  std::weak_ptr<Collider> player_col;
};

#endif