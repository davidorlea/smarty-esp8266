#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>
#include "AbstractActuator.hpp"

#define SMARTY_RELAY_CALLBACK_TYPE std::function<void (bool relayChanged)>

class SmartyRelay : virtual public SmartyAbstractActuator {
public:
  enum class State : uint8_t {
    ON = HIGH,
    OFF = LOW
  };
  SmartyRelay(const char*, const uint8_t, const State);
  void setActivateCallback(SMARTY_RELAY_CALLBACK_TYPE);
  void setDeactivateCallback(SMARTY_RELAY_CALLBACK_TYPE);
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
  SMARTY_RELAY_CALLBACK_TYPE _activateCallback;
  SMARTY_RELAY_CALLBACK_TYPE _deactivateCallback;
};
