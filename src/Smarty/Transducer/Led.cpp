#include "Led.hpp"

SmartyLed::SmartyLed(const char* name, const uint8_t port, const State state)
: SmartyAbstractActuator(name)
, _port(port)
, _restState(state) {
}

bool SmartyLed::setup() {
  pinMode(_port, OUTPUT);
  digitalWrite(_port, (uint8_t) _restState);
  return true;
}

bool SmartyLed::loop() {
  return false;
}

bool SmartyLed::activate() {
  uint8_t oldState = state();
  auto newState = (uint8_t) State::ON;
  digitalWrite(_port, newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _activateCallbacks) {
    callback(oldState != newState);
  }
  return true;
}

bool SmartyLed::deactivate() {
  uint8_t oldState = state();
  auto newState = (uint8_t) State::OFF;
  digitalWrite(_port, newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _deactivateCallbacks) {
    callback(oldState != newState);
  }
  return true;
}

bool SmartyLed::toggle() {
  switch (state()) {
    case (uint8_t) State::ON:
      deactivate();
      return true;
    case (uint8_t) State::OFF:
      activate();
      return true;
    default:
      return false;
  }
}

bool SmartyLed::parseState(int state) {
  switch(state) {
    case 0:
      deactivate();
      return true;
    case 1:
      activate();
      return true;
    case 2:
      toggle();
      return true;
    default:
      return false;
  }
}

uint8_t SmartyLed::state() {
  return (uint8_t) digitalRead(_port);
}
