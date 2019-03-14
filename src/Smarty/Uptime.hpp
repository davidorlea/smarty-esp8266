#pragma once

#include <cstdint>
#include <Arduino.h>

class SmartyUptime {
public:
  void update();
  uint32_t getSeconds() const;
private:
  uint64_t _milliseconds = 0;
  uint32_t _lastUpdate = 0;
};
