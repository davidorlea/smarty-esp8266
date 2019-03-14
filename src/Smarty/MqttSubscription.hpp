#pragma once

#include <cstdlib>
#include <functional>
#include <Streaming.h>

#define SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE std::function<void (const char* topic, const char* message)>

class SmartyMqttSubscription {
public:
  static bool isValidTopic(const char*);
  explicit SmartyMqttSubscription(const char*);
  ~SmartyMqttSubscription();
  const char* getTopic();
  void setCallback(SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE);
  bool canHandle(const char*);
  void handle(const char*, const char*);
private:
  const char* _topic;
  SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE _callback;
};
