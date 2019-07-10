#include "TileMap.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Camera.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Helpers.hpp"
#include "Layer.hpp"
#include "TileSet.hpp"
#include "Types.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace Helpers;

TileMap::TileMap(GameObject& associated, const std::string& file)
    : Component(associated), tileSet(nullptr) {
  Load(file);
}

TileMap::~TileMap() {}

void TileMap::Load(const std::string& file) {
  std::ifstream infile(file);
  json j;
  infile >> j;

  height = j["height"].get<int>();
  width = j["width"].get<int>();
  tileHeight = j["tileheight"].get<int>();
  tileWidth = j["tilewidth"].get<int>();
  layers = j["layers"].get<std::vector<Layer>>();

  auto properties = j["properties"];

  for (auto p : properties) {
    std::string key = p["name"].get<std::string>();
    if (key == "initialPositionX") {
      initialPosition.x = p["value"].get<int>();
    } else if (key == "initialPositionY") {
      initialPosition.y = p["value"].get<int>();
    }
  }

  // the last layer is a logical layer
  depth = layers.size() - 1;

  auto tilesets = j["tilesets"].get<std::vector<json>>();
  if (tilesets.size() == 0) {
    throw std::invalid_argument("Invalid map, no tilesets");
  }

  auto configuredTileset = tilesets.front();
  auto path = split(configuredTileset["image"].get<std::string>(), '/');

  TileSet* ts = new TileSet(tileWidth, tileHeight, "assets/img/" + path.back());
  tileSet = std::shared_ptr<TileSet>(ts);

  if (tileHeight != tileWidth || tileHeight % logicalTileDimension != 0) {
    throw std::invalid_argument(
        "Tiles must be squared and a multiple of logicalTileDimension");
  }

  logicalFactor = tileHeight / logicalTileDimension;
  logicalHeight = height * logicalFactor;
  logicalWidth = width * logicalFactor;
  initialPosition.x *= logicalFactor;
  initialPosition.y *= logicalFactor;

  // a layer logica deve sempre ser a ultima, mais acima
  auto& logical_layer = layers.back().data;

  walkable = std::vector<std::vector<bool>>(logicalHeight);
  for (int i = 0; i < logicalHeight; i++) {
    walkable[i] = std::vector<bool>(logicalWidth);

    for (int j = 0; j < logicalWidth; j++) {
      int idx = (i / logicalFactor * width) + j / logicalFactor;
      int equivalent = logical_layer[idx];
      walkable[i][j] = (equivalent != 0);
    }
  }

  // default parallax factor
  for (unsigned i = 0; i < layers.size(); i++) {
    layerParallax.push_back({0, 0});
  }
}

void TileMap::SetTileSet(std::shared_ptr<TileSet> tileSet) {
  this->tileSet = tileSet;
}

void TileMap::SetParallax(unsigned layer, float xFactor, float yFactor) {
  if (layer >= layers.size()) {
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

void TileMap::RenderLayer(unsigned layer, int cameraX, int cameraY) {
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
  for (unsigned i = 0; i < layers.size(); i++) {
    RenderLayer(i, Camera::pos.x, Camera::pos.y);
  }
}

Vec2 TileMap::GetInitialPosition() { return initialPosition; }

int TileMap::GetWidth() { return width; }

int TileMap::GetHeight() { return height; }

int TileMap::GetLogicalWidth() { return logicalWidth; }

int TileMap::GetLogicalHeight() { return logicalHeight; }

int TileMap::GetLogicalTileDimension() { return logicalTileDimension; }

bool TileMap::CanWalk(int x, int y) {
  if (x < 0 || x >= logicalWidth || y < 0 || y >= logicalHeight) {
    return false;
  }
  return walkable[y][x];
}

bool TileMap::NoDoors(int x, int y) {
  if (x < 0 || x >= logicalWidth || y < 0 || y >= logicalHeight) {
    return false;
  }
  return no_doors[y][x];
}

bool TileMap::Is(Types type) const { return type == this->Type; }

int TileMap::GetDepth() { return depth; }

void TileMap::Update(float dt) {}

void TileMap::SetWalk(int x, int y, bool value) { walkable[y][x] = value; }

void TileMap::MergeWalkable() { no_doors = walkable; }