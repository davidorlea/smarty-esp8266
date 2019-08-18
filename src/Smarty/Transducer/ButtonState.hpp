#pragma once

#include <cstdint>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "AbstractState.hpp"

class SmartyButtonState : public SmartyAbstractState {
public:
  enum class State : int {
    ON = LOW,
    OFF = HIGH
  };
  void setButtonState(State buttonState);
  JsonObject& applyNestedJson(JsonObject&) override;
private:
  State _buttonState{};
};
