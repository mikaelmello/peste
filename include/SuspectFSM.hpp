#ifndef SUSPECTFSM_H
#define SUSPECTFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define NO_RAGE_BIAS 0
#define RAGE_NUMERIC_LIMIT 10
#define POP_REQUEST_TIME 3

#include "Antagonist.hpp"
#include "GameObject.hpp"
#include "IFSM.hpp"
#include "Timer.hpp"

class SuspectFSM : public IFSM {
 public:
  SuspectFSM(GameObject& object);

  ~SuspectFSM();

  void OnStateEnter();

  void OnStateExecution(float dt);

  void OnStateExit();

  void Update(float dt);

 private:
  int rage_bias;

  bool cooldown_lock;

  unsigned stack_original_size;

  Vec2 initial;

  Timer bias_update_timer;

  Timer pop_request_timer;

  Timer cooldown;

  std::weak_ptr<Antagonist> ant;
};

#endif