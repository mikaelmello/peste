#include "TileMap.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Camera.hpp"
#include "Component.hpp"
#include "GameData.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Layer.hpp"
#include "TileSet.hpp"
#include "json.hpp"

using json = nlohmann::json;

TileMap::TileMap(GameObject& associated, const std::string& file,
                 TileSet* tileSet)
    : Component(associated), tileSet(tileSet) {
  Load(file);
}

void TileMap::Load(const std::string& file) {
  std::ifstream infile(file);
  json j;
  infile >> j;

  height = j["height"].get<int>();
  width = j["width"].get<int>();
  tileHeight = j["tileheight"].get<int>();
  tileWidth = j["tilewidth"].get<int>();
  layers = j["layers"].get<std::vector<Layer>>();

  // default parallax factor
  for (int i = 0; i < layers.size(); i++) {
    layerParallax.push_back({0, 0});
  }
}

void TileMap::SetTileSet(TileSet* tileSet) { this->tileSet = tileSet; }

void TileMap::SetParallax(int layer, float xFactor, float yFactor) {
  if (layer < 0 || layer >= layers.size()) {
    throw std::invalid_argument("Invalid layer to add parallax factors");
  }

  layerParallax[layer] = {xFactor, yFactor};
}

int& TileMap::At(int x, int y, int z) {
  int index = 0;
  index += width * y;
  index += x;

  return layers[z].data[index];
}

void TileMap::RenderLayer(int layer, int cameraX, int cameraY) {
  if (!layers[layer].visible) {
    return;
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int x = i * tileSet->GetTileWidth() - cameraX -
              cameraX * layerParallax[layer].x;
      int y = j * tileSet->GetTileHeight() - cameraY -
              cameraY * layerParallax[layer].y;

      int tileIndex = (unsigned)At(i, j, layer);

      if (tileIndex > 0) {
        tileSet->RenderTile(tileIndex - 1, x, y);
      }
    }
  }
}

void TileMap::Render() {
  for (int i = 0; i < layers.size(); i++) {
    RenderLayer(i, Camera::pos.x, Camera::pos.y);
  }
}

int TileMap::GetWidth() { return width; }

int TileMap::GetHeight() { return height; }

bool TileMap::Is(GameData::Types type) const { return type == this->Type; }

int TileMap::GetDepth() { return layers.size(); }

void TileMap::Update(float dt) {}
