#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Streaming.h>
#include "Firmware.hpp"
#include "MqttPublication.hpp"
#include "MqttSubscription.hpp"
#include "Uptime.hpp"
#include "Wifi.hpp"

#define MQTT_RECONNECT_INTERVAL (1000UL * 60) // 60 seconds
#define MQTT_STATUS_INTERVAL (1000UL * 60 * 5) // 5 minutes

class SmartyMqtt {
public:
  SmartyMqtt(SmartyFirmware&, SmartyUptime&, SmartyWifi&);
  void setServer(const char*, const uint16_t);
  void setClientId(const char*);
  void setSystemTopic(const char*);
  void setup();
  void loop();
private:
  WiFiClient _wifiClient;
  PubSubClient _pubSubClient;
  SmartyFirmware& _firmware;
  SmartyUptime& _uptime;
  SmartyWifi& _wifi;
  const char* _serverUrl;
  uint16_t _serverPort;
  const char* _clientId;
  const char* _systemTopic;
  unsigned long _lastConnectionAttempt;
  unsigned long _lastStatusPublish;
  void _connect();
  void _callback(char*, byte*, unsigned int);
  void _publishSystem();
  void _publishJson(const char*, JsonObject&);
  void _publish(const char*, const char*);
};
