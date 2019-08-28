#pragma once

#include "AbstractJsonSerializable.hpp"

class SmartySystem : public SmartyAbstractJsonSerializable {
public:
  void setName(const char*);
  JsonObject& toJson(JsonObject&) override;
private:
  const char* _name = nullptr;
};
