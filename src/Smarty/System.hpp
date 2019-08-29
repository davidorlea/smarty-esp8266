#pragma once

#include "AbstractJsonSerializable.hpp"
#include "SystemConfig.hpp"

class SmartySystem : public SmartyAbstractJsonSerializable {
public:
  void setup(SmartySystemConfig&);
  JsonObject& toJson(JsonObject&) override;
private:
  const char* _name = nullptr;
};
