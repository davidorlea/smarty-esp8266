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
  SmartyRelay(const char*, uint8_t, State);
  bool setup() override;
  bool loop() override;
  bool activate() override;
  bool deactivate() override;
  bool toggle() override;
  bool parseState(int) override;
  uint8_t state() override;
private:
  const uint8_t _port;
  const State _restState;
};
