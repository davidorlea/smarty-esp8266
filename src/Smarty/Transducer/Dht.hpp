#pragma once

#include <cstdint>
#include <functional>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "AbstractSensor.hpp"
#include "DhtState.hpp"

class SmartyDHT : public SmartyAbstractSensor {
public:
  enum class Type {
    DHT11 = DHT::DHT11,
    DHT22 = DHT::DHT22
  };
  static const unsigned long DHT_READ_DELAY = 60000;
  SmartyDHT(const char*, uint8_t, Type);
  bool setup() override;
  bool loop() override;
  JsonObject& toJson(JsonBuffer&) override;
private:
  SmartyDHTState _state{};
  DHT _dht{};
  const uint8_t _port;
  const Type _type;
  unsigned long _lastReadTime = 0;
};
