#include "BinaryActuator.hpp"

SmartyBinaryActuator::SmartyBinaryActuator(const char* name, const uint8_t port, const Wiring wiring)
: SmartyAbstractActuator(name)
, _port(port)
, _wiring(wiring) {
}

bool SmartyBinaryActuator::setup() {
  pinMode(_port, OUTPUT);
  digitalWrite(_port, (uint8_t) ((_wiring == Wiring::REGULAR) ? LOW : HIGH));
  return true;
}

bool SmartyBinaryActuator::loop() {
  return false;
}

bool SmartyBinaryActuator::activate() {
  int oldState = digitalRead(_port);
  int newState = (_wiring == Wiring::REGULAR) ? HIGH : LOW;
  digitalWrite(_port, (uint8_t) newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _activateCallbacks) {
    callback(oldState != newState);
  }
  return oldState != newState;
}

bool SmartyBinaryActuator::deactivate() {
  int oldState = digitalRead(_port);
  int newState = (_wiring == Wiring::REGULAR) ? LOW : HIGH;
  digitalWrite(_port, (uint8_t) newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _deactivateCallbacks) {
    callback(oldState != newState);
  }
  return oldState != newState;
}

bool SmartyBinaryActuator::toggle() {
  switch (_readState()) {
    case SmartyBinaryActuatorState::State::OFF:
      return activate();
    case SmartyBinaryActuatorState::State::ON:
      return deactivate();
    default:
      return false;
  }
}

bool SmartyBinaryActuator::parseState(int state) {
  switch((SmartyBinaryActuatorState::State) state) {
    case SmartyBinaryActuatorState::State::OFF:
      return deactivate();
    case SmartyBinaryActuatorState::State::ON:
      return activate();
    case SmartyBinaryActuatorState::State::TOGGLE:
      return toggle();
    default:
      return false;
  }
}

JsonObject& SmartyBinaryActuator::toJson(JsonBuffer& jsonBuffer) {
  JsonObject& rootJson = SmartyAbstractActuator::toJson(jsonBuffer);
  rootJson["type"] = "binaryActuator";
  _state.setBinaryActuatorState(_readState());
  _state.applyNestedJson(rootJson);
  return rootJson;
}

SmartyBinaryActuatorState::State SmartyBinaryActuator::_readState() {
  switch (digitalRead(_port)) {
    case LOW:
      return (_wiring == Wiring::REGULAR) ? SmartyBinaryActuatorState::State::OFF : SmartyBinaryActuatorState::State::ON;
    case HIGH:
      return (_wiring == Wiring::REGULAR) ? SmartyBinaryActuatorState::State::ON : SmartyBinaryActuatorState::State::OFF;
    default:
      return SmartyBinaryActuatorState::State::UNKNOWN;
  }
}
