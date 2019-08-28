#include "BinaryActuatorState.hpp"

void SmartyBinaryActuatorState::setBinaryActuatorState(SmartyBinaryActuatorState::State binaryActuatorState) {
  _binaryActuatorState = binaryActuatorState;
}

JsonObject& SmartyBinaryActuatorState::toJson(JsonObject& rootJson) {
  JsonObject& stateJson = rootJson.createNestedObject("state");
  stateJson["binaryActuator"] = (int) this->_binaryActuatorState;
  return stateJson;
}
