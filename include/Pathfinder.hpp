#ifndef PATHFINDER_H
#define PATHFINDER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <set>
#include <vector>
#include "TileMap.hpp"
#include "Vec2.hpp"

namespace Pathfinder {

typedef std::pair<double, std::pair<int, int>> f_and_cell;

struct Cell {
  Cell()
      : f(std::numeric_limits<double>::max()), closed(false), parent({-1, -1}) {}

  Cell(double f, bool c, std::pair<int, int> p) : f(f), closed(c), parent(p) {}

  double f;

  bool closed;

  std::pair<int, int> parent;
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
  Astar(GameObject& object, Heuristic* h, TileMap* tm);

  Astar(GameObject& object, TileMap* tm);

  ~Astar();

  std::vector<Vec2> Run(Vec2& start, Vec2& dest);

 private:
  GameObject& object;

  Heuristic* heuristic;

  TileMap* tm;

  Cell** details;

  // Funções auxiliares:
  void Search(std::vector<Vec2>& path, std::pair<int, int>& start,
              std::pair<int, int>& dest);

  bool CanWalk(std::pair<int, int>& p);

  void TracePath(std::pair<int, int>& dest, std::vector<Vec2>& path);

  void SetFValue(std::pair<int, int>& p, float f_value);

  void SetParent(std::pair<int, int>& leaf, std::pair<int, int>& parent);

  void Close(std::pair<int, int>& p);

  bool IsIn(std::set<f_and_cell>& open, std::pair<int, int> p);

  bool Closed(std::pair<int, int>& p);

  bool Shorter(int cost, std::pair<int, int>& p, std::pair<int, int>& d);

  std::vector<std::pair<int, int>> Neighbours(std::pair<int, int> p);
};

}  // namespace Pathfinder

#endif
