#include "System.hpp"

void SmartySystem::setName(const char* name) {
  _name = name;
}

JsonObject& SmartySystem::toJson(JsonObject&rootJson) {
  rootJson["name"] = _name;
  return rootJson;
}
