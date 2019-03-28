#include "Relay.hpp"

SmartyRelay::SmartyRelay(const char* name, const uint8_t port, const Wiring wiring)
: SmartyAbstractActuator(name)
, _port(port)
, _wiring(wiring) {
}

bool SmartyRelay::setup() {
  pinMode(_port, OUTPUT);
  digitalWrite(_port, (uint8_t) ((_wiring == Wiring::REGULAR) ? LOW : HIGH));
  return true;
}

bool SmartyRelay::loop() {
  return false;
}

bool SmartyRelay::activate() {
  int oldState = digitalRead(_port);
  int newState = (_wiring == Wiring::REGULAR) ? HIGH : LOW;
  digitalWrite(_port, (uint8_t) newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _activateCallbacks) {
    callback(oldState != newState);
  }
  return oldState != newState;
}

bool SmartyRelay::deactivate() {
  int oldState = digitalRead(_port);
  int newState = (_wiring == Wiring::REGULAR) ? LOW : HIGH;
  digitalWrite(_port, (uint8_t) newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _deactivateCallbacks) {
    callback(oldState != newState);
  }
  return oldState != newState;
}

bool SmartyRelay::toggle() {
  switch ((State) state()) {
    case State::OFF:
      return activate();
    case State::ON:
      return deactivate();
    default:
      return false;
  }
}

bool SmartyRelay::parseState(int state) {
  switch((State) state) {
    case State::OFF:
      return deactivate();
    case State::ON:
      return activate();
    case State::TOGGLE:
      return toggle();
    default:
      return false;
  }
}

uint8_t SmartyRelay::state() {
  switch (digitalRead(_port)) {
    case LOW:
      return (uint8_t) ((_wiring == Wiring::REGULAR) ? State::OFF : State::ON);
    case HIGH:
      return (uint8_t) ((_wiring == Wiring::REGULAR) ? State::ON : State::OFF);
    default:
      return (uint8_t) State::UNKNOWN;
  }
}
