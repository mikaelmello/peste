#ifndef PATHFINDER_H
#define PATHFINDER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <vector>
#include "Vec2.hpp"

namespace Pathfinder {

class Node {
 public:
  Node(Vec2 position, int cost = 1);

  Node(float x, float y, int cost = 1);

  ~Node();

  void Visit();

  void Close();

  int GetCost();

  int GetScore();

  int GetTotalScore();

  bool isVisited();

  bool isClosed();

  Vec2 GetPosition();

  Node& GetParent();

 private:
  Vec2 position;

  int cost;

  int score;

  int total_score;

  bool visited;

  bool closed;

  Node* parent;
};

class Heuristic {
 protected:
  Heuristic();

  ~Heuristic();

  float virtual Compare(Node& a, Node& b) = 0;
};

class Manhattan : public Heuristic {
 public:
  float Compare(Node& a, Node& b);
};

class Euclidian : public Heuristic {
 public:
  float Compare(Node& a, Node& b);
};

class ScoreHeap {
 public:
  bool Compare(Node& a, Node& b);
};
/*
class Astar {
 public:
  Astar(Heuristic& heuristic);

  ~Astar();

  std::vector<Vec2> Search(Node& start, Node& goal);

 private:
  Node& fillHeap(ScoreHeap& heap, Node& start, Node& goal);

  Heuristic& heuristic;
};*/

}  // namespace Pathfinder

#endif
