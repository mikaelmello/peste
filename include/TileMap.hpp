#ifndef TILEMAP_H
#define TILEMAP_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include <vector>
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Layer.hpp"
#include "TileSet.hpp"
#include "Vec2.hpp"

class TileMap : public Component {
 public:
  TileMap(GameObject& associated, const std::string& file);

  ~TileMap();

  void Load(const std::string& file);

  void SetTileSet(TileSet* tileSet);

  int& At(int x, int y, int z = 0);

  void Render();

  void RenderLayer(int layer, int cameraX = 0, int cameraY = 0);

  bool Is(GameData::Types type) const override;

  void Update(float dt) override;

  int GetWidth();

  int GetHeight();

  int GetDepth();

  void SetParallax(int layer, float xFactor, float yFactor);

  const GameData::Types Type = GameData::Types::TileMap;

 private:
  TileSet* tileSet;

  int height;
  int width;
  int tileHeight;
  int tileWidth;
  std::vector<Vec2> layerParallax;
  std::vector<Layer> layers;
};

#endif
