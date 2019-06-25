#ifndef SUSPECTFSM_H
#define SUSPECTFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define NO_RAGE_BIAS 0
#define RAGE_NUMERIC_LIMIT 10
#define POP_REQUEST_TIME 3

#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Timer.hpp"

class SuspectFSM : public IFSM {
 public:
  SuspectFSM(GameObject& object);

  ~SuspectFSM();

  void OnStateEnter();

  void OnStateExecution();

  void OnStateExit();

  void Update(float dt);

 private:
  int rage_bias;

  Vec2 initial;

  std::pair<int, std::vector<Vec2>> path;

  Timer bias_update_timer;

  Timer pop_request_timer;
};

#endif