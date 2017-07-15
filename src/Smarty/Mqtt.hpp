#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Streaming.h>
#include "Firmware.hpp"
#include "MqttPublication.hpp"
#include "MqttSubscription.hpp"
#include "Transducer/AbstractActuator.hpp"
#include "Transducer/AbstractSensor.hpp"
#include "Uptime.hpp"
#include "Wifi.hpp"

#define SMARTY_MQTT_RECONNECT_INTERVAL (1000UL * 60) // 60 seconds
#define SMARTY_MQTT_STATUS_INTERVAL (1000UL * 60 * 5) // 5 minutes

class SmartyMqtt {
public:
  SmartyMqtt(SmartyFirmware&, SmartyUptime&, SmartyWifi&);
  ~SmartyMqtt();
  void setHost(const char*);
  void setPort(const uint16_t);
  void setUsername(const char*);
  void setPassword(const char*);
  void setClientId(const char*);
  void setBaseTopic(const char*);
  void setup();
  void loop();
  void publishJson(const char*, JsonObject&, bool = false);
  void publish(const char*, const char*, bool = false);
private:
  WiFiClient _wifiClient;
  PubSubClient _pubSubClient;
  SmartyFirmware& _firmware;
  SmartyUptime& _uptime;
  SmartyWifi& _wifi;
  const char* _host = nullptr;
  uint16_t _port = 1883;
  const char* _username = nullptr;
  const char* _password = nullptr;
  const char* _clientId = nullptr;
  const char* _baseTopic = nullptr;
  unsigned long _lastConnectionAttempt;
  unsigned long _lastStatusPublish;
  std::vector<SmartyMqttPublication*> _publications;
  std::vector<SmartyMqttSubscription*> _subscriptions;
  void _addCustomPublication(SmartyAbstractActuator* actuator);
  void _addCustomPublication(SmartyAbstractSensor* sensor);
  void _addCustomSubscription(SmartyAbstractActuator* actuator);
  void _connect();
  void _callback(char*, byte*, unsigned int);
  void _publishSystem();
};
