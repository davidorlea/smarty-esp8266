#include "Relay.hpp"

SmartyRelay::SmartyRelay(const char* name, const uint8_t port, const State state)
: SmartyAbstractActuator(name)
, _port(port)
, _restState(state)
, _activateCallback(nullptr)
, _deactivateCallback(nullptr)
, _toggleCallback(nullptr) {
}

void SmartyRelay::setActivateCallback(SMARTY_RELAY_CALLBACK_TYPE callback) {
  _activateCallback = callback;
}

void SmartyRelay::setDeactivateCallback(SMARTY_RELAY_CALLBACK_TYPE callback) {
  _deactivateCallback = callback;
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
  uint8_t newState = (uint8_t) State::ON;
  digitalWrite(_port, newState);
  if (_activateCallback != nullptr) {
    _activateCallback(oldState != newState);
  }
  return true;
}

bool SmartyRelay::deactivate() {
  uint8_t oldState = state();
  uint8_t newState = (uint8_t) State::OFF;
  digitalWrite(_port, newState);
  if (_deactivateCallback != nullptr) {
    _deactivateCallback(oldState != newState);
  }
  return true;
}

bool SmartyRelay::toggle() {
  switch (state()) {
    case (uint8_t) State::ON:
      deactivate();
      break;
    case (uint8_t) State::OFF:
      activate();
      break;
    default:
      return false;
  }
  if (_toggleCallback != nullptr) {
    _toggleCallback(true);
  }
  return true;
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

int SmartyRelay::state() {
  return digitalRead(_port);
}
