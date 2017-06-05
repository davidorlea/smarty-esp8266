#pragma once

#include <cstdlib> // abort()
#include <cstring> // strcmp()
#include <functional>
#include <vector>
#include <Streaming.h>

#define SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE std::function<void (const char* topic, const char* message)>

class SmartyMqttSubscription {
public:
  static std::vector<SmartyMqttSubscription*>* getList();
  static bool isValidTopic(const char*);
  SmartyMqttSubscription(const char*);
  ~SmartyMqttSubscription();
  const char* getTopic();
  void setCallback(SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE);
  bool canHandle(const char*);
  void handle(const char*, const char*);
private:
  static std::vector<SmartyMqttSubscription*>* _list;
  std::vector<SmartyMqttSubscription*>::size_type _listIndex;
  const char* _topic;
  SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE _callback;
};
