#include "Layer.hpp"
#include "json.hpp"

void to_json(nlohmann::json& j, const Layer& p) {}

void from_json(const nlohmann::json& j, Layer& p) {
  j.at("data").get_to(p.data);
  j.at("height").get_to(p.height);
  j.at("width").get_to(p.width);
  j.at("visible").get_to(p.visible);
}