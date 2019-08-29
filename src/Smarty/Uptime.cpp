#include "Uptime.hpp"

void SmartyUptime::update() {
  uint32_t now = millis();
  _milliseconds += (now - _lastUpdate);
  _lastUpdate = now;
}

uint32_t SmartyUptime::getSeconds() const {
  return (uint32_t) (_milliseconds / 1000ULL);
}

JsonObject& SmartyUptime::toJson(JsonObject& rootJson) {
  rootJson["uptime"] = getSeconds();
  return rootJson;
}
