#pragma once

#include <functional>
#include <vector>
#include <Arduino.h>
#include "AbstractSensor.hpp"

#define SMARTY_BUTTON_DEBOUNCE_DELAY 50
#define SMARTY_BUTTON_CALLBACK_TYPE std::function<void (uint8_t buttonState)>

class SmartyButton : public SmartyAbstractSensor {
public:
  enum class Mode : uint8_t {
    PUSH = 0,
    SWITCH = 1
  };
  SmartyButton(const char*, const uint8_t, const Mode);
  void setCallback(SMARTY_BUTTON_CALLBACK_TYPE);
  bool setup();
  bool loop();
  int state();
private:
  const uint8_t _port;
  const Mode _mode;
  uint8_t _lastButtonState;
  unsigned long _debounceDelay;
  unsigned long _lastDebounceTime;
  SMARTY_BUTTON_CALLBACK_TYPE _callback;
};
