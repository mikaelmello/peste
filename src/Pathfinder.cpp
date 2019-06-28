#include "Pathfinder.hpp"
#include "Collider.hpp"
#include "Game.hpp"

Pathfinder::Astar::Astar(GameObject& o, Heuristic* h, TileMap* tm)
    : object(o), heuristic(h), tm(tm) {
  rows = tm->GetLogicalHeight();
  cols = tm->GetLogicalWidth();
}

Pathfinder::Astar::Astar(GameObject& o, TileMap* tm)
    : Astar(o, new Diagonal(), tm) {}

Pathfinder::Astar::~Astar() { delete heuristic; }

std::vector<Vec2> Pathfinder::Astar::Run(Vec2& s, Vec2& d) {
  std::pair<int, int> start = {s.x, s.y};
  std::pair<int, int> dest = {d.x, d.y};

  if (!CanWalk(start)) {
    throw std::invalid_argument("Pathfinder: current position is invalid");
  }

  if (!CanWalk(dest)) {
    throw std::invalid_argument("Pathfinder: desired destination is invalid.");
  }

  details = std::vector<Cell>(rows * cols);

  std::vector<Vec2> path;
  Search(path, dest, start);

  return path;
}

int Pathfinder::Astar::index(int i, int j) { return (i * rows) + j; }

void Pathfinder::Astar::Search(std::vector<Vec2>& path,
                               std::pair<int, int>& start,
                               std::pair<int, int>& dest) {
  details[index(start.first, start.second)] = Cell(0, 0, false, start);

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
      if (counter == 4) cost = 1.4;
      counter++;

      if (!CanWalk(neighbour) || Closed(neighbour)) {
        continue;
      }

      if (Shorter(cost, current.second, neighbour)) {
        double f_value = details[index(neighbour.first, neighbour.second)].g +
                         heuristic->Distance(neighbour, dest);
        SetFValue(neighbour, f_value);
        SetParent(neighbour, current.second);

        if (!IsIn(neighbour)) {
          SetOpened(neighbour);
          open.insert({f_value, neighbour});
        }
      }
    }
  }
}

bool Pathfinder::Astar::CanWalk(std::pair<int, int>& p) {
  if (p.first < 0 || p.first >= cols || p.second < 0 || p.second >= rows) {
    return false;
  }

  auto colliderCpt = object.GetComponent(ColliderType);
  auto spriteCpt = object.GetComponent(SpriteType);

  if (!colliderCpt || !spriteCpt) {
    throw std::runtime_error("Nao tem collider nem sprite no path finder");
  }

  auto sprite = std::dynamic_pointer_cast<Sprite>(spriteCpt);
  auto collider = std::dynamic_pointer_cast<Collider>(colliderCpt);
  int tileDim = tm->GetLogicalTileDimension();

  int cellsWidth = round(collider->box.w / tileDim);
  if (cellsWidth % 2 == 1) cellsWidth--;
  int cellsHeight = round(collider->box.h / tileDim);

  int x = p.first - cellsWidth / 2;
  int y = p.second - cellsHeight;

  for (int i = x; i <= (x + cellsWidth); i++) {
    for (int j = y; j <= (y + cellsHeight); j++) {
      if (!tm->CanWalk(i, j)) {
        return false;
      }
    }
  }

  return true;
}

void Pathfinder::Astar::TracePath(std::pair<int, int>& dest,
                                  std::vector<Vec2>& path) {
  std::pair<int, int> aux = dest;

  while (!(details[index(aux.first, aux.second)].parent == aux)) {
    aux = details[index(aux.first, aux.second)].parent;
    path.emplace_back(aux.first, aux.second);
  }
}

void Pathfinder::Astar::SetFValue(std::pair<int, int>& p, float f_value) {
  details[index(p.first, p.second)].f = f_value;
}

void Pathfinder::Astar::SetParent(std::pair<int, int>& leaf,
                                  std::pair<int, int>& parent) {
  details[index(leaf.first, leaf.second)].parent = parent;
}

void Pathfinder::Astar::Close(std::pair<int, int>& p) {
  details[index(p.first, p.second)].closed = true;
}

void Pathfinder::Astar::SetOpened(std::pair<int, int> p) {
  details[index(p.first, p.second)].opened = true;
}

bool Pathfinder::Astar::IsIn(std::pair<int, int> p) {
  return details[index(p.first, p.second)].opened;
}

bool Pathfinder::Astar::Closed(std::pair<int, int>& p) {
  return details[index(p.first, p.second)].closed;
}

bool Pathfinder::Astar::Shorter(float cost, std::pair<int, int>& s,
                                std::pair<int, int>& d) {
  bool flag = details[index(s.first, s.second)].g + cost <
              details[index(d.first, d.second)].g;

  if (flag) {
    details[index(d.first, d.second)].g =
        details[index(s.first, s.second)].g + cost;
  }
  return flag;
}

std::vector<std::pair<int, int>> Pathfinder::Astar::Neighbours(
    std::pair<int, int> p) {
  int i = p.first;
  int j = p.second;

  return {{i - 1, j},     {i + 1, j},     {i, j + 1},     {i, j - 1},
          {i - 1, j + 1}, {i - 1, j - 1}, {i + 1, j + 1}, {i + 1, j - 1}};
}
