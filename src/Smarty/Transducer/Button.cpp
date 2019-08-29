#include "Button.hpp"

SmartyButton::SmartyButton(const char* name, const uint8_t port, const Mode mode)
: SmartyAbstractSensor(name)
, _port(port)
, _mode(mode) {
}

bool SmartyButton::setup() {
  pinMode(_port, INPUT_PULLUP);
  _currentButtonState = _readState();
  _lastButtonState = _currentButtonState;
  return true;
}

bool SmartyButton::loop() {
  auto buttonState = _readState();
  unsigned long now = millis();
  if (buttonState != _lastButtonState) {
    _lastDebounceTime = now;
  }
  if ((now - _lastDebounceTime) > _debounceDelay && buttonState != _currentButtonState) {
    if (_mode == Mode::SWITCH || buttonState == SmartyButtonState::State::ON) {
      for (SMARTY_SENSOR_CALLBACK_TYPE callback : _stateCallbacks) {
        callback();
      }
    }
    _currentButtonState = buttonState;
  }
  _lastButtonState = buttonState;
  return true;
}

JsonObject& SmartyButton::toJson(JsonBuffer& jsonBuffer) {
  JsonObject& rootJson = SmartyAbstractSensor::toJson(jsonBuffer);
  rootJson["type"] = "button";
  switch (_mode) {
  case Mode::PUSH:
    rootJson["mode"] = "push";
    break;
  case Mode::SWITCH:
    rootJson["mode"] = "switch";
    break;
  }
  _state.setButtonState(_readState());
  _state.toJson(rootJson);
  return rootJson;
}

SmartyButtonState::State SmartyButton::_readState() {
  return (SmartyButtonState::State)digitalRead(_port);
}
