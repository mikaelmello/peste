#include "Component.hpp"
#include <memory>
#include "GameObject.hpp"

Component::Component(GameObject& associated) : associated(associated) {}

Component::~Component() {}

void Component::Start() {}

void Component::NotifyCollision(std::shared_ptr<GameObject> other) {}
