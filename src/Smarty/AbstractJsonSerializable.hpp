#pragma once

#include <ArduinoJson.h>

class SmartyAbstractJsonSerializable {
public:
  JsonObject& toJson(JsonBuffer& jsonBuffer) {
    JsonObject& rootJson = jsonBuffer.createObject();
    return toJson(rootJson);
  }
  virtual JsonObject& toJson(JsonObject&) = 0;
};
