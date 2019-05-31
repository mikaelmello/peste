#include "Pathfinder.hpp"
#include "Game.hpp"
#include "iostream"

Vec2 Pathfinder::next_point(Vec2& p1, Vec2& p2, Vec2& start, Vec2& goal,
                            int id) {
  float m = (goal.y - start.y) / (goal.x - start.x);
  float b = (-m) * goal.x + goal.y;

  std::cout << "y = " << m << "x + " << b << std::endl;

  float point_1 = std::abs((m * p1.x + b) - p1.y);
  float point_2 = std::abs((m * p2.x + b) - p2.y);

  std::cout << point_1 << " " << point_2 << std::endl;
  // getchar();
  return point_1 < point_2 ? p1 : p2;
}

float Pathfinder::Manhattan::Distance(int ax, int ay, int bx, int by) {
  // return abs(ax - bx) + abs(ay - by);
  return std::sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
}

Pathfinder::Astar::Astar(Manhattan* heuristic, TileMap* tilemap)
    : heuristic(heuristic), tilemap(tilemap) {}

std::vector<Vec2> Pathfinder::Astar::Run(Vec2& a, Vec2& b) {
  std::vector<Vec2> path;
  Search(path, a, a, b);
  return path;
}

#include <iostream>
int id = 0;
void Pathfinder::Astar::Search(std::vector<Vec2>& path, Vec2& start, Vec2& p,
                               Vec2& goal, int id) {
  if ((p.x == goal.x && p.y == goal.y) || !tilemap->CanWalk(goal.x, goal.y))
    return;

  std::vector<Vec2> neighbours{{p.x + 1, p.y},     {p.x - 1, p.y},
                               {p.x, p.y + 1},     {p.x, p.y - 1},
                               {p.x + 1, p.y + 1}, {p.x + 1, p.y - 1},
                               {p.x - 1, p.y + 1}, {p.x - 1, p.y - 1}};
  Vec2 s;
  float smaller_distance = 10000000.0f;

  for (auto neighbour : neighbours) {
    if (tilemap->CanWalk((int)neighbour.x, (int)neighbour.y)) {
      float distance =
          heuristic->Distance(neighbour.x, neighbour.y, goal.x, goal.y);

      if (distance <= smaller_distance) {
        if (distance == smaller_distance) {
          std::cout << "OEEEEEEEEEEEEEEEEE" << std::endl;
          s = Pathfinder::next_point(s, neighbour, start, goal, path.size());
        } else {
          smaller_distance = distance;
          s = neighbour;
        }
      }
    }
  }

  path.push_back(s);
  std::cout << path.size() << std::endl;
  return Search(path, start, s, goal, id + 1);
}