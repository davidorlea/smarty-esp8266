#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "AbstractJsonSerializable.hpp"

class SmartyHardware : public SmartyAbstractJsonSerializable {
public:
  using SmartyAbstractJsonSerializable::toJson;
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(2) + 128;
  JsonObject& toJson(JsonObject&) override;
};
