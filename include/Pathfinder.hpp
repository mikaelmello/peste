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
      : f(std::numeric_limits<double>::max()),
        g(std::numeric_limits<double>::max()),
        closed(false),
        opened(false),
        parent({-1, -1}) {}

  Cell(double f, double g, bool c, std::pair<int, int> p)
      : f(f), g(g), closed(c), opened(false), parent(p) {}

  double f, g;

  bool closed;

  bool opened;

  std::pair<int, int> parent;
};

struct Heuristic {
  double virtual Distance(std::pair<int, int> a, std::pair<int, int> b) = 0;
  virtual ~Heuristic(){};
};

struct Euclidian : public Heuristic {
  inline double Distance(std::pair<int, int> a, std::pair<int, int> b) {
    return std::sqrt((a.first - b.first) * (a.first - b.first) +
                     (a.second - b.second) * (a.second - b.second));
  }
};

struct Manhattan : public Heuristic {
  inline double Distance(std::pair<int, int> a, std::pair<int, int> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
  }
};

struct Diagonal : public Heuristic {
  inline double Distance(std::pair<int, int> a, std::pair<int, int> b) {
    return std::max(std::abs(a.first - b.first), std::abs(a.second - b.second));
  }
};

class Astar {
 public:
  Astar(GameObject& object, std::shared_ptr<Heuristic> h, TileMap* tm);

  Astar(GameObject& object, TileMap* tm);

  ~Astar();

  void ToggleMaxSearch();

  std::vector<Vec2> Run(Vec2& start, Vec2& dest);

 private:
  GameObject& object;

  std::shared_ptr<Heuristic> heuristic;

  TileMap* tm;

  std::vector<Cell> details;

  bool max_search;

  int rows;

  int cols;

  int index(int i, int j);

  int GetGValue(std::pair<int, int>& p);

  void Search(std::vector<Vec2>& path, std::pair<int, int>& start,
              std::pair<int, int>& dest);

  void TracePath(std::pair<int, int>& dest, std::vector<Vec2>& path);

  void SetFValue(std::pair<int, int>& p, float f_value);

  void SetParent(std::pair<int, int>& leaf, std::pair<int, int>& parent);

  void SetOpened(std::pair<int, int>& p);

  void Close(std::pair<int, int>& p);

  bool IsIn(std::pair<int, int>& p);

  bool Closed(std::pair<int, int>& p);

  bool Shorter(float cost, std::pair<int, int>& p, std::pair<int, int>& d);

  std::vector<std::pair<int, int>> Neighbours(std::pair<int, int>& p);
};

}  // namespace Pathfinder

#endif
