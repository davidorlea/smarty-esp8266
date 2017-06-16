#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>
#include "AbstractActuator.hpp"

class SmartyRelay : virtual public SmartyAbstractActuator {
public:
  enum class State : uint8_t {
    ON = HIGH,
    OFF = LOW
  };
  SmartyRelay(const char*, const uint8_t, const State);
  bool setup();
  bool loop();
  bool activate();
  bool deactivate();
  bool toggle();
  bool parseState(int);
  uint8_t state();
private:
  const uint8_t _port;
  const State _restState;
};
