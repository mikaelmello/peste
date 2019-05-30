#ifndef PATHFINDER_H
#define PATHFINDER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <vector>
#include "TileMap.hpp"
#include "Vec2.hpp"

namespace Pathfinder {

class Manhattan {
 public:
  float Distance(int ax, int ay, int bx, int by);
};

class Astar {
 public:
  Astar(Manhattan* heuristic, TileMap* tilemap);

  std::vector<Vec2> Run(Vec2& start, Vec2& goal);

 private:
  void Search(std::vector<Vec2>& path, Vec2& p, Vec2& goal, int id = 0);

  Manhattan* heuristic;

  TileMap* tilemap;
};

}  // namespace Pathfinder

#endif
