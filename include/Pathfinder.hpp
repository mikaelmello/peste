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
  Cell() : f(FLT_MAX), parent_i(-1), parent_j(-1), closed(false) {}

  Cell(double f, int p_i, int p_j, bool c)
      : f(f), parent_i(p_i), parent_j(p_j), closed(c) {}

  int parent_i;

  int parent_j;

  double f;

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
  Astar(GameObject& object, Heuristic& h, TileMap* tm);

  ~Astar();

  std::vector<Vec2> Run(Vec2& start, Vec2& dest);

 private:
  GameObject& object;

  Heuristic& heuristic;

  TileMap* tm;

  Cell** details;

  void Search(std::vector<Vec2>& path, std::pair<int, int>& start,
              std::pair<int, int>& dest);

  bool CanWalk(std::pair<int, int>& p);

  void TracePath(std::pair<int, int>& dest, std::vector<Vec2>& path);

  inline void SetFValue(std::pair<int, int>& p, float f_value) {
    details[p.first][p.second].f = f_value;
  }

  inline void SetParent(std::pair<int, int>& leaf,
                        std::pair<int, int>& parent) {
    details[leaf.first][leaf.second].parent_i = parent.first;
    details[leaf.first][leaf.second].parent_j = parent.second;
  }

  inline void Close(std::pair<int, int>& p) {
    int i = p.first;
    int j = p.second;
    details[i][j].closed = true;
  }

  inline bool IsIn(std::set<f_and_cell>& open, std::pair<int, int> p) {
    for (auto& s : open) {
      if (s.second.first == p.first && s.second.second == p.first) {
        return true;
      }
    }
    return false;
  }

  inline bool Closed(std::pair<int, int>& p) {
    return details[p.first][p.second].closed;
  }

  inline bool Shorter(int cost, std::pair<int, int>& p,
                      std::pair<int, int>& d) {
    return cost + heuristic.Distance(p, d) < details[p.first][p.second].f;
  }

  inline std::vector<std::pair<int, int>> Neighbours(std::pair<int, int> p) {
    int i = p.first;
    int j = p.second;

    return {{i - 1, j},     {i + 1, j},     {i, j + 1},     {i, j - 1},
            {i - 1, j + 1}, {i - 1, j - 1}, {i + 1, j + 1}, {i + 1, j - 1}};
  }
};

}  // namespace Pathfinder

#endif
