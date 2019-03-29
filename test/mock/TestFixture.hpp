#pragma once

#include <memory>
#include <gtest/gtest.h>
#include "ArduinoMock.hpp"

class TestFixture : public ::testing::Test {
public:
  TestFixture();
  ~TestFixture() override;
  static ArduinoMock* arduinoMock;
};
