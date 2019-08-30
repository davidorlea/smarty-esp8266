#pragma once

#include "AbstractJsonSerializable.hpp"
#include "SystemConfig.hpp"

class SmartySystem : public SmartyAbstractJsonSerializable {
public:
  using SmartyAbstractJsonSerializable::toJson;
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(1) + 64;
  void setup(SmartySystemConfig&);
  JsonObject& toJson(JsonObject&) override;
private:
  const char* _name = nullptr;
};
