#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "AbstractActuator.hpp"
#include "BinaryActuatorState.hpp"

class SmartyBinaryActuator : virtual public SmartyAbstractActuator {
public:
  enum class Wiring {
    REGULAR,
    INVERSE
  };
  SmartyBinaryActuator(const char*, uint8_t, Wiring);
  bool setup() override;
  bool loop() override;
  bool activate() override;
  bool deactivate() override;
  bool toggle() override;
  bool parseState(int) override;
  JsonObject& toJson(JsonBuffer&) override;
private:
  SmartyBinaryActuatorState _state{};
  const uint8_t _port;
  const Wiring _wiring;
  SmartyBinaryActuatorState::State _readState();
};
