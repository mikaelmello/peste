#ifndef IFSM_H
#define IFSM_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GameObject.hpp"

class IFSM {
 public:
  IFSM(GameObject& object);

  ~IFSM();

  inline bool PopRequested() { return pop_requested; }

  void virtual OnStateEnter() = 0;

  void virtual OnStateExecution() = 0;

  void virtual OnStateExit() = 0;

  void virtual Update(float dt) = 0;

 protected:
  struct Walkable {
    bool can_walk;

    Vec2 walkable;

    Walkable(): can_walk(false) {}
    
    Walkable(bool cw, Vec2 w): can_walk(cw), walkable(w) {}
  };
  // Retorna uma posição da collider->box de @param pivot
  // que seja andável para @param object.
  static Walkable GetWalkable(GameObject& object, GameObject& pivot);

  GameObject& object;

  bool pop_requested;
};

#endif
