#include "Pathfinder.hpp"
#include "Game.hpp"

Vec2 Pathfinder::next_point(Vec2& p1, Vec2& p2, Vec2& start, Vec2& goal) {
  float m = (goal.y - start.y) / (goal.x - start.x);
  float b = (-m) * goal.x + goal.y;

  float point_1 = std::abs((m * p1.x + b) - p1.y);
  float point_2 = std::abs((m * p2.x + b) - p2.y);

  return point_1 < point_2 ? p1 : p2;
}

int Pathfinder::Manhattan::Distance(int ax, int ay, int bx, int by) {
  return abs(ax - bx) + abs(ay - by);
}

Pathfinder::Astar::Astar(Manhattan* heuristic, TileMap* tilemap)
    : heuristic(heuristic), tilemap(tilemap) {}

std::vector<Vec2> Pathfinder::Astar::Run(Vec2& a, Vec2& b) {
  std::vector<Vec2> path;
  Search(path, a, a, b);
  return path;
}

// Retorna os vizinhos do ponto p com execeção do ponto x
// x = path[path.size() - 2] que descreve o ponto pai do ponto p.
std::vector<Vec2> Pathfinder::neighbours(std::vector<Vec2>& path, Vec2& p,
                                         Vec2 father_p) {
  std::vector<Vec2> neighbours;
  std::vector<Vec2> deltas = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                              {1, 0},   {-1, 1}, {0, 1},  {1, 1}};

  for (auto delta : deltas) {
    if ((father_p.x != p.x + delta.x) || (father_p.y != p.y + delta.y)) {
      neighbours.push_back(p + delta);
    }
  }
  return neighbours;
}

void Pathfinder::Astar::Search(std::vector<Vec2>& path, Vec2& start, Vec2& p,
                               Vec2& goal) {
  if ((p.x == goal.x && p.y == goal.y) || !tilemap->CanWalk(goal.x, goal.y))
    return;

  std::vector<Vec2> neighbours =
      path.size() < 2 ? Pathfinder::neighbours(path, p)
                      : Pathfinder::neighbours(path, p, path[path.size() - 2]);

  Vec2 s;
  int smaller_distance = std::numeric_limits<int>::max();

  for (auto neighbour : neighbours) {
    if (tilemap->CanWalk(neighbour.x, neighbour.y)) {
      float distance =
          heuristic->Distance(neighbour.x, neighbour.y, goal.x, goal.y);

      if (distance < smaller_distance) {
        smaller_distance = distance;
        s = Pathfinder::next_point(s, neighbour, start, goal);
      }
    }
  }

  path.push_back(s);
  return Search(path, start, s, goal);
}