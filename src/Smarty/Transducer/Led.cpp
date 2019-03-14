#include "Led.hpp"

SmartyLed::SmartyLed(const char* name, const uint8_t port, const State state)
: SmartyAbstractActuator(name)
, _port(port)
, _restState(state)
, _virtualState(state) {
}

bool SmartyLed::setup() {
  pinMode(_port, OUTPUT);
  digitalWrite(_port, (uint8_t) _restState);
  return true;
}

bool SmartyLed::loop() {
  if (_lastBlinkTime) {
    unsigned long now = millis();
    if ((_blinkRate == BlinkRate::SLOW &&
         now - _lastBlinkTime >= LED_BLINK_INTERVAL_SLOW) ||
        (_blinkRate == BlinkRate::FAST &&
         now - _lastBlinkTime >= LED_BLINK_INTERVAL_FAST)) {
      _lastBlinkTime = now;
      digitalWrite(_port, (uint8_t) !state());
    }
  }
  return true;
}

bool SmartyLed::activate() {
  if (_isBlinking()) {
    _virtualState = State::ON;
    return false;
  }
  uint8_t oldState = state();
  auto newState = (uint8_t) State::ON;
  digitalWrite(_port, newState);
  for (SMARTY_ACTUATOR_CALLBACK_TYPE callback: _activateCallbacks) {
    callback(oldState != newState);
  }
  return true;
}

bool SmartyLed::deactivate() {
  if (_isBlinking()) {
    _virtualState = State::OFF;
    return false;
  }
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

bool SmartyLed::startBlinking(BlinkRate blinkRate) {
  if (!_isBlinking()) {
    _virtualState = (State) state();
  }
  _blinkRate = blinkRate;
  _lastBlinkTime = millis();
  digitalWrite(_port, (uint8_t) !state());
  return true;
}

bool SmartyLed::stopBlinking() {
  if (!_isBlinking()) {
    return false;
  }
  _lastBlinkTime = 0UL;
  digitalWrite(_port, (uint8_t) _virtualState);
  return true;
}

uint8_t SmartyLed::state() {
  return (uint8_t) digitalRead(_port);
}

bool SmartyLed::_isBlinking() {
  return _lastBlinkTime != 0;
}
