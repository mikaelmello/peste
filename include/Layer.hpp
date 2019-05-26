#ifndef LAYER_H
#define LAYER_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                  INCLUDES E DEFINES
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <vector>
#include "json.hpp"

struct Layer {
  std::vector<int> data;
  int height;
  int width;
  bool visible;
};

void to_json(nlohmann::json& j, const Layer& p);

void from_json(const nlohmann::json& j, Layer& p);

#endif
