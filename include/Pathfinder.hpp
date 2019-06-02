#ifndef PATHFINDER_H
#define PATHFINDER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <vector>
#include "TileMap.hpp"
#include "Vec2.hpp"

namespace Pathfinder {

std::vector<Vec2> neighbours(std::vector<Vec2>& path, Vec2& p,
                             Vec2 father_p = Vec2(0, 0));

Vec2 next_point(Vec2& p1, Vec2& p2, Vec2& start, Vec2& goal);

class Manhattan {
 public:
  int Distance(int ax, int ay, int bx, int by);
};

class Astar {
 public:
  Astar(Manhattan* heuristic, TileMap* tilemap);

  std::vector<Vec2> Run(Vec2& start, Vec2& goal);

 private:
  void Search(std::vector<Vec2>& path, Vec2& start, Vec2& p, Vec2& goal);

  Manhattan* heuristic;

  TileMap* tilemap;
};

}  // namespace Pathfinder

#endif
