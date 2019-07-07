#include "Dialogue.hpp"
#include <memory>
#include <string>
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"

Dialogue::Dialogue(GameObject& associated) : Component(associated) {
  Sprite* sprite = new Sprite(associated, "assets/img/dialogue.png");
  associated.AddComponent(sprite);
}

Dialogue::~Dialogue() {}

void Dialogue::NotifyCollision(std::shared_ptr<GameObject> other) {}

void Dialogue::Start() {}

void Dialogue::Update(float dt) {}

void Dialogue::Render() {}

bool Dialogue::Is(Types type) const { return type == this->Type; }
