#pragma once

#include <cstdint>
#include <functional>
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
  JsonObject& toJson(JsonBuffer&) override;
  bool fromJson(StaticJsonBufferBase&, const char*) override;
private:
  SmartyBinaryActuatorState _state{};
  const uint8_t _port;
  const Wiring _wiring;
  bool _parseState(int);
  SmartyBinaryActuatorState::State _readState();
};
