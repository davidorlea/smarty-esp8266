#pragma once

#include <cstdlib>
#include <vector>
#include <Streaming.h>

class SmartyMqttPublication {
public:
  static std::vector<SmartyMqttPublication*>* getList();
  static bool isValidTopic(const char*);
  SmartyMqttPublication(const char*);
  ~SmartyMqttPublication();
  const char* getTopic();
  void setMessage(const char*);
  const char* getMessage();
  void ready(bool = true);
  bool isReady();
private:
  static std::vector<SmartyMqttPublication*>* _list;
  std::vector<SmartyMqttPublication*>::size_type _listIndex;
  const char* _topic = nullptr;
  const char* _message = nullptr;
  bool _ready = false;
};
