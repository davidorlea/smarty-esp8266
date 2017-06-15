#pragma once

#include <cstdint>
#include <ArduinoJson.h>
#include <FS.h>
#include <Streaming.h>

#define SMARTY_CONFIG_FILE "/config.json"
#define SMARTY_CONFIG_MAX_FILE_SIZE 1000
#define SMARTY_CONFIG_MAX_JSON_SIZE JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4)

#define SMARTY_CONFIG_MAX_WIFI_SSID_LENGTH 32 + 1
#define SMARTY_CONFIG_MAX_WIFI_PASSWORD_LENGTH 64 + 1
#define SMARTY_CONFIG_MAX_MQTT_HOST_LENGTH 255 + 1
#define SMARTY_CONFIG_MAX_MQTT_CLIENT_ID_LENGTH 23 + 1
#define SMARTY_CONFIG_MAX_MQTT_BASE_TOPIC_LENGTH 128 + 1

class SmartyConfig {
public:
  void setup();
  const char* getWifiSSID() const;
  const char* getWifiPassword() const;
  const char* getMqttHost() const;
  uint16_t getMqttPort() const;
  const char* getMqttClientId() const;
  const char* getMqttBaseTopic() const;
private:
  char _wifiSSID[SMARTY_CONFIG_MAX_WIFI_SSID_LENGTH] = "";
  char _wifiPassword[SMARTY_CONFIG_MAX_WIFI_PASSWORD_LENGTH] = "";
  char _mqttHost[SMARTY_CONFIG_MAX_MQTT_HOST_LENGTH] = "";
  uint16_t _mqttPort = 0;
  char _mqttClientId[SMARTY_CONFIG_MAX_MQTT_CLIENT_ID_LENGTH] = "";
  char _mqttBaseTopic[SMARTY_CONFIG_MAX_MQTT_BASE_TOPIC_LENGTH] = "";
};
