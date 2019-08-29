#pragma once

#include <cstdint>
#include <ArduinoJson.h>
#include <FS.h>
#include <Streaming.h>
#include "MqttConfig.hpp"
#include "SystemConfig.hpp"
#include "WifiConfig.hpp"

#define SMARTY_CONFIG_FILE "/config.json"
#define SMARTY_CONFIG_MAX_FILE_SIZE 1000
#define SMARTY_CONFIG_MAX_JSON_SIZE JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(6)

class SmartyConfig : public SmartyMqttConfig, public SmartySystemConfig, public SmartyWifiConfig {
public:
  static const unsigned int CONFIG_MAX_NAME_LENGTH = 255 + 1;
  static const unsigned int CONFIG_MAX_WIFI_SSID_LENGTH = 32 + 1;
  static const unsigned int CONFIG_MAX_WIFI_PASSWORD_LENGTH = 64 + 1;
  static const unsigned int CONFIG_MAX_WIFI_HOSTNAME_LENGTH = 64 + 1;
  static const unsigned int CONFIG_MAX_MQTT_HOST_LENGTH = 255 + 1;
  static const unsigned int CONFIG_MAX_MQTT_USERNAME_LENGTH = 32 + 1;
  static const unsigned int CONFIG_MAX_MQTT_PASSWORD_LENGTH = 32 + 1;
  static const unsigned int CONFIG_MAX_MQTT_CLIENT_ID_LENGTH = 23 + 1;
  static const unsigned int CONFIG_MAX_MQTT_BASE_TOPIC_LENGTH = 128 + 1;
  void setup();
  const char* getName() const override;
  const char* getWifiSSID() const override;
  const char* getWifiPassword() const override;
  const char* getWifiHostname() const override;
  const char* getMqttHost() const override;
  uint16_t getMqttPort() const override;
  const char* getMqttUsername() const override;
  const char* getMqttPassword() const override;
  const char* getMqttClientId() const override;
  const char* getMqttBaseTopic() const override;
private:
  char _name[CONFIG_MAX_NAME_LENGTH] = "";
  char _wifiSSID[CONFIG_MAX_WIFI_SSID_LENGTH] = "";
  char _wifiPassword[CONFIG_MAX_WIFI_PASSWORD_LENGTH] = "";
  char _wifiHostname[CONFIG_MAX_WIFI_HOSTNAME_LENGTH] = "";
  char _mqttHost[CONFIG_MAX_MQTT_HOST_LENGTH] = "";
  uint16_t _mqttPort = 0;
  char _mqttUsername[CONFIG_MAX_MQTT_USERNAME_LENGTH] = "";
  char _mqttPassword[CONFIG_MAX_MQTT_PASSWORD_LENGTH] = "";
  char _mqttClientId[CONFIG_MAX_MQTT_CLIENT_ID_LENGTH] = "";
  char _mqttBaseTopic[CONFIG_MAX_MQTT_BASE_TOPIC_LENGTH] = "";
};
