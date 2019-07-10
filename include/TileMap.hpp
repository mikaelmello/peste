#ifndef TILEMAP_H
#define TILEMAP_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include <vector>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Layer.hpp"
#include "TileSet.hpp"
#include "Types.hpp"
#include "Vec2.hpp"

class TileMap : public Component {
 public:
  TileMap(GameObject& associated, const std::string& file);

  ~TileMap();

  void Load(const std::string& file);

  void SetTileSet(std::shared_ptr<TileSet> tileSet);

  int& At(int x, int y, int z = 0);

  void Render();

  void RenderLayer(unsigned layer, int cameraX = 0, int cameraY = 0);

  bool Is(Types type) const override;

  void Update(float dt) override;

  bool CanWalk(int x, int y);

  bool NoDoors(int x, int y);

  int GetLogicalWidth();

  int GetLogicalHeight();

  int GetWidth();

  int GetHeight();

  int GetDepth();

  int GetLogicalTileDimension();

  Vec2 GetInitialPosition();

  void SetWalk(int x, int y, bool value);

  void SetParallax(unsigned layer, float xFactor, float yFactor);

  void MergeWalkable();

  const Types Type = TileMapType;

 private:
  std::shared_ptr<TileSet> tileSet;

  int height;
  int width;
  int depth;
  int tileWidth;
  int tileHeight;

  int logicalHeight;
  int logicalWidth;
  int logicalFactor;
  int logicalTileDimension = 8;

  Vec2 initialPosition;

  std::vector<Vec2> layerParallax;
  std::vector<Layer> layers;

  std::vector<std::vector<bool>> walkable;
  std::vector<std::vector<bool>> no_doors;
};

#endif
