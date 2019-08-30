#pragma once

#include "AbstractJsonSerializable.hpp"

class SmartyFirmware : public  SmartyAbstractJsonSerializable {
public:
  using SmartyAbstractJsonSerializable::toJson;
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(3) + 64;
  const char* version = nullptr;
  int buildTime = 0;
  JsonObject& toJson(JsonObject&) override;
};
