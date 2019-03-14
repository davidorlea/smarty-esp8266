#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>
#include "AbstractSensor.hpp"

#define SMARTY_BUTTON_DEBOUNCE_DELAY 150

class SmartyButton : public SmartyAbstractSensor {
public:
  enum class State : uint8_t {
    ON = LOW,
    OFF = HIGH
  };
  enum class Mode : uint8_t {
    PUSH = 0,
    SWITCH = 1
  };
  SmartyButton(const char*, uint8_t, Mode);
  bool setup() override;
  bool loop() override;
  uint8_t state() override;
private:
  const uint8_t _port;
  const Mode _mode;
  uint8_t _lastButtonState;
  unsigned long _debounceDelay;
  unsigned long _lastDebounceTime;
};
