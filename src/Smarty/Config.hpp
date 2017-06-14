#pragma once

#include <cstdint>
#include <ArduinoJson.h>
#include <FS.h>
#include <Streaming.h>

#define SMARTY_CONFIG_FILE "/config.json"
#define SMARTY_CONFIG_MAX_FILE_SIZE 1000
#define SMARTY_CONFIG_MAX_JSON_SIZE JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4)

class SmartyConfig {
public:
  void setup();
  const char* getWifiSSID() const;
  const char* getWifiPassword() const;
  const char* getMqttHost() const;
  uint16_t getMqttPort() const;
  const char* getMqttClientId() const;
  const char* getMqttSystemTopic() const;
private:
  const char* _wifiSSID = nullptr;
  const char* _wifiPassword = nullptr;
  const char* _mqttHost = nullptr;
  uint16_t _mqttPort = 1883;
  const char* _mqttClientId = nullptr;
  const char* _mqttSystemTopic = nullptr;
};
