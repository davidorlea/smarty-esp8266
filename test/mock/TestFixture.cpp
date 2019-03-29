#include "TestFixture.hpp"

ArduinoMock* TestFixture::arduinoMock = nullptr;

TestFixture::TestFixture() {
  arduinoMock = new ArduinoMock();
}

TestFixture::~TestFixture() {
  delete arduinoMock;
}
