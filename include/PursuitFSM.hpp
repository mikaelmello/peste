#ifndef PURSUITFSM_H
#define PURSUITFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Antagonist.hpp"
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Pathfinder.hpp"
#include "Timer.hpp"

class PursuitFSM : public IFSM {
 public:
  PursuitFSM(GameObject& object);

  ~PursuitFSM();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

 private:
  unsigned stack_original_size;

  std::unique_ptr<Pathfinder::Astar> pf;

  Vec2 initial;

  Timer timer;

  std::weak_ptr<Antagonist> ant;
};

#endif