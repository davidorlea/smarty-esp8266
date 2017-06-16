#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>
#include "AbstractActuator.hpp"

#define SMARTY_LED_BLINK_INTERVAL_SLOW 800
#define SMARTY_LED_BLINK_INTERVAL_FAST 400

class SmartyLed : virtual public SmartyAbstractActuator {
public:
  enum class State : uint8_t {
    ON = LOW,
    OFF = HIGH
  };
  enum class BlinkRate : uint8_t {
    SLOW = 0,
    FAST = 1
  };
  SmartyLed(const char*, const uint8_t, const State);
  bool setup();
  bool loop();
  bool activate();
  bool deactivate();
  bool toggle();
  bool parseState(int);
  bool startBlinking(BlinkRate = BlinkRate::FAST);
  bool stopBlinking();
  uint8_t state();
private:
  const uint8_t _port;
  const State _restState;
  State _virtualState;
  BlinkRate _blinkRate;
  unsigned long _lastBlinkTime;
  bool _isBlinking();
};
