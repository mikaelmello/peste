#ifndef IFSM_H
#define IFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GameObject.hpp"
#include "Timer.hpp"

class IFSM {
 public:
  IFSM(GameObject& object);

  virtual ~IFSM();

  inline bool PopRequested() { return pop_requested; }

  void virtual OnStateEnter() = 0;

  void virtual OnStateExecution(float dt) = 0;

  void virtual OnStateExit() = 0;

  void virtual Update(float dt) = 0;

  struct Walkable {
    bool can_walk;

    Vec2 walkable;

    Walkable() : can_walk(false) {}

    Walkable(bool cw, Vec2 w) : can_walk(cw), walkable(w) {}
  };
  // Retorna uma posição da collider->box de @param pivot
  // que seja andável para IFSM::object.
  static Walkable GetWalkable(GameObject& object, GameObject& pivot);

 protected:
  bool UpdatePosition(float dt);

  GameObject& object;

  bool pop_requested;

  float speed = 30;

  float accumulated = 0;

 private:
  Timer timer;
};

#endif
