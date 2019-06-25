#ifndef PURSUITFSM_H
#define PURSUITFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
  std::pair<int, std::vector<Vec2>> path;

  std::unique_ptr<Pathfinder::Astar> pf;

  Timer timer;
};

#endif