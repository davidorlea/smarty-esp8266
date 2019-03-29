#include "Button.hpp"

SmartyButton::SmartyButton(const char* name, const uint8_t port, const Mode mode)
: SmartyAbstractSensor(name)
, _port(port)
, _mode(mode) {
}

bool SmartyButton::setup() {
  pinMode(_port, INPUT_PULLUP);
  _currentButtonState = (uint8_t) digitalRead(_port);
  _lastButtonState = _currentButtonState;
  return true;
}

bool SmartyButton::loop() {
    uint8_t buttonState = state();
    unsigned long now = millis();
    if (buttonState != _lastButtonState) {
        _lastDebounceTime = now;
    }
    if ((now - _lastDebounceTime) > _debounceDelay && buttonState != _currentButtonState) {
        if (_mode == Mode::SWITCH || buttonState == (uint8_t) State::ON) {
            for (SMARTY_SENSOR_CALLBACK_TYPE callback: _stateCallbacks) {
                callback(buttonState);
            }
        }
        _currentButtonState = buttonState;
    }
    _lastButtonState = buttonState;
    return true;
}

uint8_t SmartyButton::state() {
  return (uint8_t) digitalRead(_port);
}
