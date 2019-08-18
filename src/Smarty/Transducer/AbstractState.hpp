#pragma once

#include <ArduinoJson.h>

class SmartyAbstractState {
public:
  virtual JsonObject& applyNestedJson(JsonObject&) = 0;
};
