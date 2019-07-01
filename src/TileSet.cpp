#include "TileSet.hpp"

TileSet::TileSet(int tileWidth, int tileHeight, const std::string& file)
    : tileSet(gameObject, file), tileWidth(tileWidth), tileHeight(tileHeight) {
  if (tileSet.IsOpen()) {
    rows = tileSet.GetHeight() / tileHeight;
    columns = tileSet.GetWidth() / tileWidth;
  }
}

void TileSet::RenderTile(unsigned int index, float x, float y) {
  if ((long long)index >= (rows * columns)) {
    return;
  }

  int clipX = (index % columns) * tileWidth;
  int clipY = (index / columns) * tileHeight;

  tileSet.SetClip(clipX, clipY, tileWidth, tileHeight);
  tileSet.Render(x, y);
}

int TileSet::GetTileHeight() { return tileHeight; }

int TileSet::GetTileWidth() { return tileWidth; }
