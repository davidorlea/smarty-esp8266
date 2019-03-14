#include "Button.hpp"

SmartyButton::SmartyButton(const char* name, const uint8_t port, const Mode mode)
: SmartyAbstractSensor(name)
, _port(port)
, _mode(mode) {
}

bool SmartyButton::setup() {
  pinMode(_port, INPUT_PULLUP);
  return true;
}

bool SmartyButton::loop() {
  uint8_t currentButtonState = state();
  if (currentButtonState != _lastButtonState) {
    unsigned long now = millis();
    if ((now - _lastDebounceTime) > _debounceDelay) {
      for (SMARTY_SENSOR_CALLBACK_TYPE callback: _stateCallbacks) {
        callback(currentButtonState);
      }
      if (_mode == Mode::SWITCH) {
          _lastButtonState = currentButtonState;
      }
    }
    _lastDebounceTime = now;
  }
  return true;
}

uint8_t SmartyButton::state() {
  return (uint8_t) digitalRead(_port);
}
