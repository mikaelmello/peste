#define INCLUDE_SDL
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "Helpers.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Collider.hpp"
#include "SDL_include.h"
#include "Sprite.hpp"

template <typename Out>
void Helpers::split(const std::string& s, char delim, Out result) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<std::string> Helpers::split(const std::string& s, char delim) {
  std::vector<std::string> elems;
  Helpers::split(s, delim, std::back_inserter(elems));
  return elems;
}

std::vector<std::string> Helpers::split(const std::string& text,
                                        const std::string& delims) {
  std::vector<std::string> tokens;
  std::size_t start = text.find_first_not_of(delims), end = 0;

  while ((end = text.find_first_of(delims, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = text.find_first_not_of(delims, end);
  }
  if (start != std::string::npos) tokens.push_back(text.substr(start));

  return tokens;
}

bool Helpers::is_whitespace(const std::string& s) {
  return std::all_of(s.begin(), s.end(), isspace);
}

float Helpers::rad_to_deg(float rad) { return rad * 180 / M_PI; }

float Helpers::deg_to_rad(float deg) { return deg * M_PI / 180; }

Helpers::Direction Helpers::combine_moves(bool up, bool down, bool left,
                                          bool right) {
  if (up && down) {
    up = false;
    down = false;
  }

  if (left && right) {
    left = false;
    right = false;
  }

  if (up) {
    if (left) {
      return Helpers::Direction::UPLEFT;
    }

    if (right) {
      return Helpers::Direction::UPRIGHT;
    }

    return Helpers::Direction::UP;
  }

  if (down) {
    if (left) {
      return Helpers::Direction::DOWNLEFT;
    }

    if (right) {
      return Helpers::Direction::DOWNRIGHT;
    }

    return Helpers::Direction::DOWN;
  }

  if (left) {
    return Helpers::Direction::LEFT;
  }

  if (right) {
    return Helpers::Direction::RIGHT;
  }

  return Helpers::Direction::NONE;
}

void Helpers::set_collider_box(GameObject& object) {
  auto sprite =
      std::dynamic_pointer_cast<Sprite>(object.GetComponent(Types::SpriteType));
  auto collider = std::dynamic_pointer_cast<Collider>(
      object.GetComponent(Types::ColliderType));

  int w = sprite->GetWidth();
  int h = sprite->GetHeight();
  Vec2 s = collider->GetScale();

  object.box.x -= (w * (1.0f - s.x) / 2.0);
  object.box.y -= (h * (1.0f - s.y) / 2.0);
}