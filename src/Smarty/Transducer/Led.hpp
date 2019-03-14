#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>
#include "AbstractActuator.hpp"

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
  static const unsigned long LED_BLINK_INTERVAL_SLOW = 800;
  static const unsigned long LED_BLINK_INTERVAL_FAST = 400;
  SmartyLed(const char*, uint8_t, State);
  bool setup() override;
  bool loop() override;
  bool activate() override;
  bool deactivate() override;
  bool toggle() override;
  bool parseState(int) override;
  bool startBlinking(BlinkRate = BlinkRate::FAST);
  bool stopBlinking();
  uint8_t state() override;
private:
  const uint8_t _port;
  const State _restState;
  State _virtualState;
  BlinkRate _blinkRate = BlinkRate::FAST;
  unsigned long _lastBlinkTime = 0;
  bool _isBlinking();
};
