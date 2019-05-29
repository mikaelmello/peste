#ifndef RECT_H
#define RECT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include "Vec2.hpp"

class Rect {
 public:
  Rect();

  Rect(float x, float y, float w, float h);

  Vec2 Center() const;

  Rect GetCentered(Vec2 pos) const;
  Rect GetCentered(float x, float y) const;

  void SetCenter(Vec2 pos);
  void SetCenter(float x, float y);

  float Distance(const Rect& rect) const;

  bool Contains(const Vec2& point) const;

  Rect& operator+=(const Vec2& rhs);
  Rect& operator=(const Rect& rhs);

  friend Rect operator+(Rect lhs, const Vec2& rhs) {
    lhs += rhs;
    return lhs;
  }

  const std::string ToString() const {
    return std::string("x:") + std::to_string(x) + std::string(" y:") +
           std::to_string(y) + std::string(" w:") + std::to_string(w) +
           std::string(" h:") + std::to_string(h) + std::string(" center: ") +
           Center().ToString();
  }

  float x;
  float y;
  float w;
  float h;
};

#endif
