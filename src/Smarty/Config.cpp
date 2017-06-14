#include "Config.hpp"

void SmartyConfig::setup() {
  SPIFFS.begin();

  File configFile = SPIFFS.open(SMARTY_CONFIG_FILE, "r");
  if (!configFile) {
    Serial << "ERROR: configuration file not found" << endl;
    return;
  }

  size_t configSize = configFile.size();

  if (configSize > SMARTY_CONFIG_MAX_FILE_SIZE) {
    Serial << "ERROR: configuration file too big" << endl;
    return;
  }

  char buffer[SMARTY_CONFIG_MAX_FILE_SIZE];
  configFile.readBytes(buffer, configSize);
  configFile.close();
  buffer[configSize] = '\0';

  StaticJsonBuffer<SMARTY_CONFIG_MAX_JSON_SIZE> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buffer);
  if (!json.success()) {
    Serial << "ERROR: configuration file invalid" << endl;
    return;
  }

  const char* jsonWifiSSID = json["wifi"]["ssid"];
  if (jsonWifiSSID) {
    strlcpy(_wifiSSID, jsonWifiSSID, SMARTY_CONFIG_MAX_WIFI_SSID_LENGTH);
  }
  const char* jsonWifiPassword = json["wifi"]["password"];
  if (jsonWifiPassword) {
    strlcpy(_wifiPassword, jsonWifiPassword, SMARTY_CONFIG_MAX_WIFI_PASSWORD_LENGTH);
  }
  const char* jsonMqttHost = json["mqtt"]["host"];
  if (jsonMqttHost) {
    strlcpy(_mqttHost, jsonMqttHost, SMARTY_CONFIG_MAX_MQTT_HOST_LENGTH);
  }
  uint16_t jsonMqttPort = json["mqtt"]["port"];
  if (jsonMqttPort) {
    _mqttPort = jsonMqttPort;
  }
  const char* jsonMqttClientId = json["mqtt"]["clientId"];
  if (jsonMqttClientId) {
    strlcpy(_mqttClientId, jsonMqttClientId, SMARTY_CONFIG_MAX_MQTT_CLIENT_ID_LENGTH);
  }
  const char* jsonMqttSystemTopic = json["mqtt"]["systemTopic"];
  if (jsonMqttSystemTopic) {
    strlcpy(_mqttSystemTopic, jsonMqttSystemTopic, SMARTY_CONFIG_MAX_MQTT_SYSTEM_TOPIC_LENGTH);
  }

  SPIFFS.end();
}

const char* SmartyConfig::getWifiSSID() const {
  return _wifiSSID;
}

const char* SmartyConfig::getWifiPassword() const {
  return _wifiPassword;
}

const char* SmartyConfig::getMqttHost() const {
  return _mqttHost;
}

uint16_t SmartyConfig::getMqttPort() const {
  return _mqttPort;
}

const char* SmartyConfig::getMqttClientId() const {
  return _mqttClientId;
}

const char* SmartyConfig::getMqttSystemTopic() const {
  return _mqttSystemTopic;
}
