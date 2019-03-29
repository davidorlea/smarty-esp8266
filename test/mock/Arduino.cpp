#include "Arduino.hpp"

unsigned long millis() {
  return TestFixture::arduinoMock->millis();
}

void pinMode(uint8_t pin, uint8_t mode) {
  return TestFixture::arduinoMock->pinMode(pin, mode);
}

int digitalRead(uint8_t pin) {
  return TestFixture::arduinoMock->digitalRead(pin);
}

void digitalWrite(uint8_t pin, uint8_t val) {
  return TestFixture::arduinoMock->digitalWrite(pin, val);
}
