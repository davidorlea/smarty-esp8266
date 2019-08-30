#pragma once

#include <Arduino.h>
#include "AbstractJsonSerializable.hpp"

class SmartyFirmware : public  SmartyAbstractJsonSerializable {
public:
  using SmartyAbstractJsonSerializable::toJson;
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(3) + 256;
  const char* version = nullptr;
  int buildTime = 0;
  JsonObject& toJson(JsonObject&) override;
};
