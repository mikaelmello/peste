
#ifdef DEBUG
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include "Camera.hpp"
#include "Game.hpp"
#endif  // DEBUG
#include <string>
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset)
    : Component(associated), scale(scale), offset(offset) {}

void Collider::Update(float dt) {
  box = associated.box;
  box.w *= scale.x;
  box.h *= scale.y;
  box.SetCenter(associated.box.Center());

  box += offset.GetRotated(Helpers::deg_to_rad(associated.angleDeg));
}

void Collider::Render() {
#ifdef DEBUG
  Vec2 center = box.Center();
  SDL_Point points[5];

  Vec2 point = (Vec2(box.x, box.y) - center)
                   .GetRotated(Helpers::deg_to_rad(associated.angleDeg)) +
               center - Camera::pos;
  points[0] = {(int)point.x, (int)point.y};
  points[4] = {(int)point.x, (int)point.y};

  point = (Vec2(box.x + box.w, box.y) - center)
              .GetRotated(Helpers::deg_to_rad(associated.angleDeg)) +
          center - Camera::pos;
  points[1] = {(int)point.x, (int)point.y};

  point = (Vec2(box.x + box.w, box.y + box.h) - center)
              .GetRotated(Helpers::deg_to_rad(associated.angleDeg)) +
          center - Camera::pos;
  points[2] = {(int)point.x, (int)point.y};

  point = (Vec2(box.x, box.y + box.h) - center)
              .GetRotated(Helpers::deg_to_rad(associated.angleDeg)) +
          center - Camera::pos;
  points[3] = {(int)point.x, (int)point.y};

  SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0,
                         SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif  // DEBUG
}

bool Collider::Is(Types type) const { return type == this->Type; }

void Collider::SetOffset(Vec2 offset) { this->offset = offset; }

void Collider::SetScale(Vec2 scale) { this->scale = scale; }

Vec2 Collider::GetOffSet() const { return offset; }

Vec2 Collider::GetScale() const { return scale; }
