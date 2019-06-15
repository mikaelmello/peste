#include "Pathfinder.hpp"
#include "Game.hpp"

void Pathfinder::Astar::TracePath(Pathfinder::Cell** details,
                                  std::pair<int, int> dest,
                                  std::vector<Vec2>& path) {
  int row = dest.first;
  int col = dest.second;

  while (!(details[row][col].parent_i == row &&
           details[row][col].parent_j == col)) {
    int temp_row = details[row][col].parent_i;
    int temp_col = details[row][col].parent_j;
    row = temp_row;
    col = temp_col;
    path.push_back({(float)row, (float)col});
  }
}

Pathfinder::Astar::Astar(Heuristic& h, TileMap* tm) : h(h), tm(tm) {}

Pathfinder::Astar::~Astar() {}

std::vector<Vec2> Pathfinder::Astar::Run(Vec2& start, Vec2& dest) {
  if (isDestination(start.x, start.y, {dest.x, dest.y})) {
    printf("Já está no destino.\n");
    return {};
  }

  if (!isValid(start.x, start.y) || !isValid(dest.x, dest.y)) {
    printf("parâmetros inválidos.\n");
    return {};
  }

  std::vector<Vec2> path;
  Search(path, {dest.x, dest.y}, {start.x, start.y});
  return path;
}

void Pathfinder::Astar::Search(std::vector<Vec2>& path,
                               std::pair<int, int> start,
                               std::pair<int, int> dest) {
  int row = tm->GetLogicalHeight();
  int col = tm->GetLogicalWidth();

  Pathfinder::Cell** details;
  details = new Pathfinder::Cell*[row];
  for (int i = 0; i < row; i++) {
    details[i] = new Pathfinder::Cell[col];
  }

  int i = start.first;
  int j = start.second;

  details[i][j] = Pathfinder::Cell(0, 0, i, j, false);

  std::set<dii> open;
  open.insert(std::make_pair(0.0f, std::make_pair(i, j)));

  while (!open.empty()) {
    dii p = *open.begin();
    open.erase(open.begin());

    i = p.second.first;
    j = p.second.second;

    details[i][j].closed = true;

    std::vector<std::pair<int, int>> neighbours = {
        {i - 1, j},     {i + 1, j},     {i, j + 1},     {i, j - 1},
        {i - 1, j + 1}, {i - 1, j - 1}, {i + 1, j + 1}, {i + 1, j - 1}};

    float cost = 1.0f;
    int count = 0;

    for (std::pair<int, int> v : neighbours) {
      if (count == 4) {
        cost = std::sqrt(2);
      }
      count++;

      if (isValid(v.first, v.second)) {
        if (isDestination(v.first, v.second, dest)) {
          details[v.first][v.second].parent_i = i;
          details[v.first][v.second].parent_j = j;
          TracePath(details, dest, path);
          return;
        } else if (!details[v.first][v.second].closed &&
                   tm->CanWalk(v.first, v.second)) {
          double g = details[i][j].g + cost;
          double h = this->h.Distance(v, dest);
          double f = g + h;

          if (details[v.first][v.first].f > f) {
            open.insert(std::make_pair(f, std::make_pair(v.first, v.second)));

            details[v.first][v.second].f = f;
            details[v.first][v.second].g = g;

            details[v.first][v.second].parent_i = i;
            details[v.first][v.second].parent_j = j;
          }
        }
      }
    }
  }

  printf("Caminho não encontrado.\n");
}