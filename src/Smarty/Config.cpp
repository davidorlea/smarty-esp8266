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
    Serial << "ERROR: configuration file not found" << endl;
    return;
  }

  char buf[SMARTY_CONFIG_MAX_FILE_SIZE];
  configFile.readBytes(buf, configSize);
  configFile.close();
  buf[configSize] = '\0';

  StaticJsonBuffer<SMARTY_CONFIG_MAX_JSON_SIZE> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf);
  if (!json.success()) {
    Serial << "ERROR: configuration file invalid" << endl;
    return;
  }

  _wifiSSID = json["wifi"]["ssid"];
  _wifiPassword = json["wifi"]["password"];
  _mqttHost = json["mqtt"]["host"];
  _mqttPort = json["mqtt"]["port"];
  _mqttSystemTopic = json["mqtt"]["systemTopic"];

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

const char *SmartyConfig::getMqttSystemTopic() const {
  return _mqttSystemTopic;
}
