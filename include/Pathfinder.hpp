#ifndef PATHFINDER_H
#define PATHFINDER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <vector>
#include "TileMap.hpp"
#include "Vec2.hpp"

namespace Pathfinder {

typedef std::pair<int, int> ii;

typedef std::pair<double, ii> dii;

struct Cell {
  Cell()
      : f(FLT_MAX),
        g(FLT_MAX),
        h(FLT_MAX),
        parent_i(-1),
        parent_j(-1),
        closed(false) {}

  Cell(double f, double g, double h, int p_i, int p_j, bool c)
      : f(f), g(g), h(h), parent_i(p_i), parent_j(p_j), closed(c) {}

  int parent_i, parent_j;

  double f, g, h;

  bool closed;
};

class Heuristic {
 public:
  double virtual Distance(std::pair<int, int> a, std::pair<int, int> b) = 0;
};

class Euclidian : public Heuristic {
 public:
  inline double Distance(std::pair<int, int> a, std::pair<int, int> b) {
    return std::sqrt((a.first - b.first) * (a.first - b.first) +
                     (a.second - b.second) * (a.second - b.second));
  }
};

class Manhattan : public Heuristic {
 public:
  inline double Distance(std::pair<int, int> a, std::pair<int, int> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
  }
};

class Diagonal : public Heuristic {
 public:
  inline double Distance(std::pair<int, int> a, std::pair<int, int> b) {
    return std::max(std::abs(a.first - b.first), std::abs(a.second - b.second));
  }
};

class Astar {
 public:
  Astar(Heuristic& h, TileMap* tm);

  ~Astar();

  std::vector<Vec2> Run(Vec2& start, Vec2& dest);

 private:
  Heuristic& h;

  TileMap* tm;

  void Search(std::vector<Vec2>& path, std::pair<int, int> start,
              std::pair<int, int> dest);

  inline bool isDestination(int row, int col, std::pair<int, int> dest) {
    return row == dest.first && col == dest.second;
  }

  inline bool isValid(int row, int col) { return tm->CanWalk(row, col); }
};

}  // namespace Pathfinder

#endif
