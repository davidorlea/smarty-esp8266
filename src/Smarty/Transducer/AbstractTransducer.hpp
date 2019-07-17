#pragma once

#include <cstdint>
#include <ArduinoJson.h>

class SmartyAbstractTransducer {
public:
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(2);
  const char* getName();
  virtual bool setup() = 0;
  virtual bool loop() = 0;
  virtual uint8_t state() = 0;
  JsonObject& toJson(JsonBuffer&);
protected:
  const char* _name;
  explicit SmartyAbstractTransducer(const char*);
};
