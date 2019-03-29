#pragma once

#include <gmock/gmock.h>

class ArduinoMock {
public:
  MOCK_METHOD0(millis, unsigned long());
  MOCK_METHOD2(pinMode, void(uint8_t pin, uint8_t mode));
  MOCK_METHOD1(digitalRead, int(uint8_t pin));
  MOCK_METHOD2(digitalWrite, void(uint8_t pin, uint8_t val));
};
