#pragma once

#include <ArduinoJson.h>

class SmartyAbstractJsonSerializable {
public:
  virtual JsonObject& toJson(JsonObject&) = 0;
};
