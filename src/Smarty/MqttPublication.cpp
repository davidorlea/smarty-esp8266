#include "MqttPublication.hpp"

std::vector<SmartyMqttPublication*>* SmartyMqttPublication::_list = nullptr;

std::vector<SmartyMqttPublication*>* SmartyMqttPublication::getList() {
  if (_list == nullptr) {
    _list = new std::vector<SmartyMqttPublication*>();
  }
  return _list;
}

bool SmartyMqttPublication::isValidTopic(const char* topic) {
  // Code from http://git.eclipse.org/c/mosquitto/org.eclipse.mosquitto.git/tree/lib/util_mosq.c
  while (topic && topic[0]) {
    if (topic[0] == '+' || topic[0] == '#') {
      return false;
    }
    topic = &topic[1];
  }
  return true;
}

SmartyMqttPublication::SmartyMqttPublication(const char* topic) {
  if (!SmartyMqttPublication::isValidTopic(topic)) {
    Serial << "ERROR: Invalid MQTT publication topic!" << endl;
    abort();
  }
  _topic = strcpy(new char[strlen(topic) + 1], topic);

  if (_list == nullptr) {
    _list = new std::vector<SmartyMqttPublication*>();
  }
  _list->push_back(this);
  _listIndex = _list->size() - 1;
}

SmartyMqttPublication::~SmartyMqttPublication() {
  _list->erase(_list->begin() + _listIndex);
  delete _topic;
}

const char *SmartyMqttPublication::getTopic() {
  return _topic;
}

void SmartyMqttPublication::setMessage(const char* message) {
  _message = message;
}

const char *SmartyMqttPublication::getMessage() {
  return _message;
}

void SmartyMqttPublication::ready(bool ready) {
  _ready = ready;
}

bool SmartyMqttPublication::isReady() {
  return _ready && _topic && _message;
}
