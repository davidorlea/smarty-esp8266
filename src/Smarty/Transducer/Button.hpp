#pragma once

#include <cstdint>
#include <functional>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "AbstractSensor.hpp"
#include "ButtonState.hpp"

class SmartyButton : public SmartyAbstractSensor {
public:
  enum class Mode {
    PUSH,
    SWITCH
  };
  static const unsigned long BUTTON_DEBOUNCE_DELAY = 50;
  SmartyButton(const char*, uint8_t, Mode);
  bool setup() override;
  bool loop() override;
  JsonObject& toJson(JsonBuffer&) override;
private:
  SmartyButtonState _state{};
  const uint8_t _port;
  const Mode _mode;
  SmartyButtonState::State _currentButtonState = SmartyButtonState::State::OFF;
  SmartyButtonState::State _lastButtonState = SmartyButtonState::State::OFF;
  unsigned long _debounceDelay = BUTTON_DEBOUNCE_DELAY;
  unsigned long _lastDebounceTime = 0;
  SmartyButtonState::State _readState();
};
