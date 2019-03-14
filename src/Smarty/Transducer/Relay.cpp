#include "Relay.hpp"

SmartyRelay::SmartyRelay(const char* name, const uint8_t port, const State state)
: SmartyAbstractActuator(name)
, _port(port)
, _restState(state) {
}

bool SmartyRelay::setup() {
  pinMode(_port, OUTPUT);
  digitalWrite(_port, (uint8_t) _restState);
  return true;
}

bool SmartyRelay::loop() {
  return false;
}

bool SmartyRelay::activate() {
  uint8_t oldState = state();
  auto newState = (uint8_t) State::ON;
  digitalWrite(_port, newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _activateCallbacks) {
    callback(oldState != newState);
  }
  return true;
}

bool SmartyRelay::deactivate() {
  uint8_t oldState = state();
  auto newState = (uint8_t) State::OFF;
  digitalWrite(_port, newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _deactivateCallbacks) {
    callback(oldState != newState);
  }
  return true;
}

bool SmartyRelay::toggle() {
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

bool SmartyRelay::parseState(int state) {
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

uint8_t SmartyRelay::state() {
  return (uint8_t) digitalRead(_port);
}
