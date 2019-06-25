#include "Pathfinder.hpp"
#include "Collider.hpp"
#include "Game.hpp"

Pathfinder::Astar::Astar(GameObject& o, Heuristic* h, TileMap* tm)
    : object(o), heuristic(h), tm(tm) {}

Pathfinder::Astar::Astar(GameObject& o, TileMap* tm)
    : Astar(o, new Diagonal(), tm) {}

Pathfinder::Astar::~Astar() { delete heuristic; }

std::vector<Vec2> Pathfinder::Astar::Run(Vec2& s, Vec2& d) {
  std::pair<int, int> start = {s.x, s.y};
  std::pair<int, int> dest = {d.x, d.y};

  if (!CanWalk(start) || !CanWalk(dest)) {
    throw std::invalid_argument(
        "Current object position or desired destination invalid");
  }

  int row = tm->GetLogicalHeight();
  int col = tm->GetLogicalWidth();

  details = new Pathfinder::Cell*[col];
  for (int i = 0; i < col; i++) {
    details[i] = new Pathfinder::Cell[row];
  }

  std::vector<Vec2> path;
  Search(path, dest, start);

  for (int i = 0; i < col; i++) {
    delete details[i];
  }

  return path;
}

void Pathfinder::Astar::Search(std::vector<Vec2>& path,
                               std::pair<int, int>& start,
                               std::pair<int, int>& dest) {
  details[start.first][start.second] = Cell(0, false, start);

  std::set<f_and_cell> open;
  open.insert(std::make_pair(0, start));

  while (!open.empty()) {
    f_and_cell current = *open.begin();  // Pega o nó com menor f do conjunto.
    open.erase(open.begin());

    Close(current.second);

    if (current.second == dest) {
      TracePath(dest, path);
      return;
    }

    std::vector<std::pair<int, int>> neighbours = Neighbours(current.second);
    int counter = 0;
    float cost = 1.0f;

    for (auto neighbour : neighbours) {
      if (counter == 4) cost = std::sqrt(2);
      counter++;

      if (!CanWalk(neighbour) || Closed(neighbour)) {
        continue;
      }

      if (Shorter(cost, neighbour, dest) || !IsIn(open, neighbour)) {
        double f_value = cost + heuristic->Distance(neighbour, dest);
        SetFValue(neighbour, f_value);
        SetParent(neighbour, current.second);

        if (!IsIn(open, neighbour)) {
          open.insert({f_value, neighbour});
        }
      }
    }
  }
}

bool Pathfinder::Astar::CanWalk(std::pair<int, int>& p) {
  auto collider = std::dynamic_pointer_cast<Collider>(
      object.GetComponent(ColliderType).lock());

  float start_w = 1.0f;
  float end_w = 1.0f;
  float start_h = 0;

  Vec2 offset = collider->GetOffSet();

  if (collider) {
    Vec2 scale = collider->GetScale();

    start_w = (1.0f - scale.x) / 2.0f;
    end_w = 1 - scale.x / 2.0f;
    start_h = (1.0f - scale.y);
  }

  int wi = start_w * object.box.w / tm->GetLogicalTileDimension() + offset.x;
  int w = end_w * object.box.w / tm->GetLogicalTileDimension() + offset.x;
  int hi = start_h * object.box.h / tm->GetLogicalTileDimension();
  int h = object.box.h / tm->GetLogicalTileDimension();

  bool can_walk = true;
  for (int i = wi; i < w && can_walk; i++) {
    for (int j = hi; j < h && can_walk; j++) {
      can_walk = tm->CanWalk(p.first + i, p.second + j);
    }
  }
  return can_walk;
}

void Pathfinder::Astar::TracePath(std::pair<int, int>& dest,
                                  std::vector<Vec2>& path) {
  std::pair<int, int> aux = dest;

  while (!(details[aux.first][aux.second].parent == aux)) {
    path.push_back({(float)aux.first, (float)aux.second});
    aux = details[aux.first][aux.second].parent;
  }
}

void Pathfinder::Astar::SetFValue(std::pair<int, int>& p, float f_value) {
  details[p.first][p.second].f = f_value;
}

void Pathfinder::Astar::SetParent(std::pair<int, int>& leaf,
                                  std::pair<int, int>& parent) {
  details[leaf.first][leaf.second].parent = parent;
}

void Pathfinder::Astar::Close(std::pair<int, int>& p) {
  int i = p.first;
  int j = p.second;
  details[i][j].closed = true;
}

bool Pathfinder::Astar::IsIn(std::set<f_and_cell>& open,
                             std::pair<int, int> p) {
  for (auto& s : open) {
    if (s.second == p) {
      return true;
    }
  }
  return false;
}

bool Pathfinder::Astar::Closed(std::pair<int, int>& p) {
  return details[p.first][p.second].closed;
}

bool Pathfinder::Astar::Shorter(int cost, std::pair<int, int>& p,
                                std::pair<int, int>& d) {
  return cost + heuristic->Distance(p, d) < details[p.first][p.second].f;
}

std::vector<std::pair<int, int>> Pathfinder::Astar::Neighbours(
    std::pair<int, int> p) {
  int i = p.first;
  int j = p.second;

  return {{i - 1, j},     {i + 1, j},     {i, j + 1},     {i, j - 1},
          {i - 1, j + 1}, {i - 1, j - 1}, {i + 1, j + 1}, {i + 1, j - 1}};
}
