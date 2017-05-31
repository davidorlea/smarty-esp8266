#pragma once

#include <Arduino.h>

class SmartyUptime {
public:
  SmartyUptime();
  void update();
  uint32_t getSeconds() const;
private:
  uint64_t _milliseconds;
  uint32_t _lastUpdate;
};
