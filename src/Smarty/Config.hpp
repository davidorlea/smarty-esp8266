#pragma once

#include <cstdint>
#include <ArduinoJson.h>
#include <FS.h>
#include <Streaming.h>

#define SMARTY_CONFIG_FILE "/config.json"
#define SMARTY_CONFIG_MAX_FILE_SIZE 1000
#define SMARTY_CONFIG_MAX_JSON_SIZE JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3)

class SmartyConfig {
public:
  void setup();
  const char* getFirmwareName() const;
  const char* getFirmwareVersion() const;
  const char* getWifiSSID() const;
  const char* getWifiPassword() const;
  const char* getMqttHost() const;
  uint16_t getMqttPort() const;
  const char *getMqttSystemTopic() const;
private:
  const char* _firmwareName;
  const char* _firmwareVersion;
  const char* _wifiSSID;
  const char* _wifiPassword;
  const char* _mqttHost;
  uint16_t _mqttPort;
  const char* _mqttSystemTopic;
};
