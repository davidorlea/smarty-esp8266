#pragma once

#include <cstddef>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <umm_malloc/umm_malloc.h>
#include "AbstractJsonSerializable.hpp"

class SmartyHardware : public SmartyAbstractJsonSerializable {
public:
  using SmartyAbstractJsonSerializable::toJson;
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + 256;
  JsonObject& toJson(JsonObject&) override;
private:
  const size_t _blockSize = 8;
  size_t _getTotalMemory();
  size_t _getTotalUsedMemory();
  size_t _getTotalAvailableMemory();
  size_t _getLargestAvailableBlock();
  inline float _getFragmentation() {
    return 100 - _getLargestAvailableBlock() * 100.0 / _getTotalAvailableMemory();
  }
};
