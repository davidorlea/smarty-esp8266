#pragma once

#include <cstdint>
#include <ArduinoJson.h>
#include "AbstractState.hpp"

class SmartyAbstractTransducer {
public:
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(10) + 128;
  const char* getName();
  virtual JsonObject& toJson(JsonBuffer&);
  virtual bool setup() = 0;
  virtual bool loop() = 0;
protected:
  const char* _name;
  explicit SmartyAbstractTransducer(const char*);
};
