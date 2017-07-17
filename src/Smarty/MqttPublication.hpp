#pragma once

#include <cstdlib>
#include <Streaming.h>

class SmartyMqttPublication {
public:
  static bool isValidTopic(const char*);
  SmartyMqttPublication(const char*);
  ~SmartyMqttPublication();
  const char* getTopic();
  void setMessage(const char*);
  const char* getMessage();
  void setRetain(bool);
  bool getRetain();
  void ready(bool = true);
  bool isReady();
private:
  const char* _topic = nullptr;
  const char* _message = nullptr;
  bool _retain = false;
  bool _ready = false;
};
