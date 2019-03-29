#pragma once

#include "HardwareSerialMock.hpp"
#include "TestFixture.hpp"

unsigned long millis();

void pinMode(uint8_t pin, uint8_t mode);

int digitalRead(uint8_t pin);

void digitalWrite(uint8_t pin, uint8_t val);
