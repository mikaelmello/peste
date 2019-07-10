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
#include "Game.hpp"
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

bool Helpers::CanWalk(GameObject& object, Vec2& p) {
  Game& game = Game::GetInstance();
  State& state = game.GetCurrentState();
  auto tilemap = state.GetCurrentTileMap();

  int rows = tilemap->GetLogicalHeight();
  int cols = tilemap->GetLogicalWidth();

  if (p.x < 0 || p.x >= cols || p.y < 0 || p.y >= rows) {
    return false;
  }

  auto colliderCpt = object.GetComponent(ColliderType);
  if (!colliderCpt) {
    throw std::runtime_error("object sem collider em Helpers::CanWalk");
  }

  auto collider = std::dynamic_pointer_cast<Collider>(colliderCpt);
  int tileDim = 8;

  int cellsWidth = round(collider->box.w / tileDim);
  int cellsHeight = round(collider->box.h / tileDim);
  if (cellsWidth % 2 == 1) cellsWidth--;

  int x = p.x - cellsWidth / 2;
  int y = p.y - cellsHeight;

  bool answer = true;

  answer &= tilemap->NoDoors(x, y);
  answer &= tilemap->NoDoors(x, y + cellsHeight);
  answer &= tilemap->NoDoors(x + cellsWidth, y);
  answer &= tilemap->NoDoors(x + cellsWidth, y + cellsHeight);
  return answer;
}

bool Helpers::CanWalk(GameObject& object, std::pair<int, int>& p) {
  Vec2 point(p.first, p.second);
  return CanWalk(object, point);
}