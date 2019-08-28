#pragma once

#include <cstdint>
#include <ArduinoJson.h>
#include "AbstractState.hpp"

class SmartyBinaryActuatorState : public SmartyAbstractState {
public:
  enum class State : int {
    OFF = 0,
    ON = 1,
    TOGGLE = 2,
    UNKNOWN = 3
  };
  void setBinaryActuatorState(State binaryActuatorState);
  JsonObject& toJson(JsonObject&) override;
private:
  State _binaryActuatorState{};
};
