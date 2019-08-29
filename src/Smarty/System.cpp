#include "System.hpp"

void SmartySystem::setup(SmartySystemConfig& config) {
  if (config.getName()[0]) {
    _name = config.getName();
  }
}

JsonObject& SmartySystem::toJson(JsonObject&rootJson) {
  rootJson["name"] = _name;
  return rootJson;
}
