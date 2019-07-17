#include "AbstractTransducer.hpp"

SmartyAbstractTransducer::SmartyAbstractTransducer(const char* name)
: _name(name) {
}

const char* SmartyAbstractTransducer::getName() {
  return _name;
}

JsonObject& SmartyAbstractTransducer::toJson(JsonBuffer& jsonBuffer) {
  JsonObject& rootJson = jsonBuffer.createObject();
  rootJson["name"] = this->getName();
  rootJson["state"] = this->state();
  return rootJson;
}
