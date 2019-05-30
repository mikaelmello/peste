#include "Pathfinder.hpp"

Pathfinder::Node::Node(Vec2 position, int cost)
    : position(position),
      cost(cost),
      visited(false),
      closed(false),
      parent(nullptr),
      score(0),
      total_score(0) {}

Pathfinder::Node::Node(float x, float y, int cost) : Node({x, y}, cost) {}

Pathfinder::Node::~Node() {}

void Pathfinder::Node::Visit() { visited = true; }

void Pathfinder::Node::Close() { closed = true; }

int Pathfinder::Node::GetCost() { return cost; }

int Pathfinder::Node::GetScore() { return score; }

int Pathfinder::Node::GetTotalScore() { return total_score; }

bool Pathfinder::Node::isVisited() { return visited; }

bool Pathfinder::Node::isClosed() { return closed; }

Vec2 Pathfinder::Node::GetPosition() { return position; }

Pathfinder::Node& Pathfinder::Node::GetParent() { return *parent; }

Pathfinder::Heuristic::Heuristic() {}

Pathfinder::Heuristic::~Heuristic() {}

float Pathfinder::Manhattan::Compare(Node& a, Node& b) {
  return std::abs(a.GetPosition().x - b.GetPosition().x) +
         std::abs(a.GetPosition().y - b.GetPosition().y);
}

float Pathfinder::Euclidian::Compare(Node& a, Node& b) {
  return (a.GetPosition() - b.GetPosition()).Magnitude();
}

bool Pathfinder::ScoreHeap::Compare(Node& a, Node& b) {
  return a.GetTotalScore() <= b.GetTotalScore();
}