#pragma once

#include <cstdint>
#include <Arduino.h>
#include "AbstractJsonSerializable.hpp"

class SmartyUptime : public SmartyAbstractJsonSerializable{
public:
  using SmartyAbstractJsonSerializable::toJson;
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(1) + 64;
  void update();
  uint32_t getSeconds() const;
  JsonObject& toJson(JsonObject&) override;
private:
  uint64_t _milliseconds = 0;
  uint32_t _lastUpdate = 0;
};
