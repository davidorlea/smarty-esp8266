#include "BinaryActuatorState.hpp"

void SmartyBinaryActuatorState::setBinaryActuatorState(SmartyBinaryActuatorState::State binaryActuatorState) {
  _binaryActuatorState = binaryActuatorState;
}

JsonObject& SmartyBinaryActuatorState::applyNestedJson(JsonObject& rootJson) {
  JsonObject& stateJson = rootJson.createNestedObject("state");
  stateJson["binaryActuator"] = (int) this->_binaryActuatorState;
  return stateJson;
}
