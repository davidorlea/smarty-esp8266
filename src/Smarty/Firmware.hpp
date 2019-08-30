#pragma once

#include "AbstractJsonSerializable.hpp"

class SmartyFirmware : public  SmartyAbstractJsonSerializable {
public:
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(3);
  const char* version = nullptr;
  int buildTime = 0;
  JsonObject& toJson(JsonObject&) override;
};
