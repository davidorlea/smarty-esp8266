#pragma once

#include <MQTT.h>

class SmartyMQTTClient : public MQTTClient {
public:
  void* parent = nullptr;
  SmartyMQTTClient(void*, int);
};
