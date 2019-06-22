#ifndef COMPONENT_H
#define COMPONENT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Types.hpp"

class GameObject;

class Component {
 public:
  Component(GameObject &associated);

  virtual ~Component();

  virtual void NotifyCollision(GameObject &other);

  virtual void Start();

  virtual void Update(float dt) = 0;

  virtual void Render() = 0;

  virtual bool Is(Types type) const = 0;

 protected:
  GameObject &associated;
};

#endif
