#pragma once

#include <cstdint>

class SmartyMqttConfig {
public:
  virtual const char* getMqttHost() const = 0;
  virtual uint16_t getMqttPort() const = 0;
  virtual const char* getMqttUsername() const = 0;
  virtual const char* getMqttPassword() const = 0;
  virtual const char* getMqttClientId() const = 0;
  virtual const char* getMqttBaseTopic() const = 0;
};
