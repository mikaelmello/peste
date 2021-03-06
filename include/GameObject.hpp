#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <memory>
#include <string>
#include <vector>
#include "Component.hpp"
#include "Rect.hpp"
#include "Types.hpp"

class State;

class GameObject {
 public:
  GameObject(double priority = 1000);

  ~GameObject();

  void Update(float dt);

  void Render();

  void Start();

  bool IsDead() const;

  void RequestDelete();

  void ReverseDelete();

  void NotifyCollision(std::shared_ptr<GameObject> other);

  void AddComponent(Component* cpt);

  void RemoveComponent(Component* cpt);

  bool IsRendering();

  inline void DisableRender() { render = false; }
  inline void EnableRender() { render = true; }
  inline void ToggleRender() { render = !render; }

  std::shared_ptr<Component> GetComponent(Types type);

  void SetPriority(double newPriority);

  Rect box;

  double angleDeg;

  double priority;

 private:
  std::vector<std::shared_ptr<Component>> components;
  bool isDead;
  bool started;
  bool render;
};

#endif
