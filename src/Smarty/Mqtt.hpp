#pragma once

#include <vector>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Streaming.h>
#include "MqttPublication.hpp"
#include "MqttSubscription.hpp"

#define SMARTY_MQTT_RECONNECT_INTERVAL (1000UL * 60) // 60 seconds
#define SMARTY_MQTT_STATUS_INTERVAL (1000UL * 60) // 60 seconds

class SmartyMqtt {
public:
  SmartyMqtt();
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
  void subscribe(const char*, const char*, SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE);
  void subscribe(const char*, SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE);
  bool isConnected();
private:
  WiFiClient _wifiClient;
  PubSubClient _pubSubClient;
  const char* _host = nullptr;
  uint16_t _port = 1883;
  const char* _username = nullptr;
  const char* _password = nullptr;
  const char* _clientId = nullptr;
  const char* _baseTopic = nullptr;
  unsigned long _lastConnectionAttempt;
  std::vector<SmartyMqttPublication*> _publications;
  std::vector<SmartyMqttSubscription*> _subscriptions;
  void _connect();
  void _callback(char*, byte*, unsigned int);
};
