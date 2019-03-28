#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>
#include "AbstractActuator.hpp"

class SmartyRelay : virtual public SmartyAbstractActuator {
public:
  enum class State : uint8_t {
    OFF = 0,
    ON = 1,
    TOGGLE = 2,
    UNKNOWN = 3
  };
  enum class Wiring {
    REGULAR,
    INVERSE
  };
  SmartyRelay(const char*, uint8_t, Wiring);
  bool setup() override;
  bool loop() override;
  bool activate() override;
  bool deactivate() override;
  bool toggle() override;
  bool parseState(int) override;
  uint8_t state() override;
private:
  const uint8_t _port;
  const Wiring _wiring;
};
