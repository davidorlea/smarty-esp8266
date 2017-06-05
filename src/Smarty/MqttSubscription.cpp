#include "MqttSubscription.hpp"

std::vector<SmartyMqttSubscription*>* SmartyMqttSubscription::_list = nullptr;

std::vector<SmartyMqttSubscription*>* SmartyMqttSubscription::getList() {
  if (_list == nullptr) {
    _list = new std::vector<SmartyMqttSubscription*>();
  }
  return _list;
}

bool SmartyMqttSubscription::isValidTopic(const char *topic) {
  char c = '\0';
	while (topic && topic[0]) {
		if (topic[0] == '+') {
			if ((c != '\0' && c != '/') || (topic[1] != '\0' && topic[1] != '/')) {
				return false;
			}
		} else if (topic[0] == '#') {
			if ((c != '\0' && c != '/')  || topic[1] != '\0') {
				return false;
			}
		}
		c = topic[0];
		topic = &topic[1];
	}
  return true;
}

SmartyMqttSubscription::SmartyMqttSubscription(const char* topic)
: _topic(topic) {
  if (!SmartyMqttSubscription::isValidTopic(topic)) {
    Serial << "ERROR: Invalid MQTT subscription topic!" << endl;
    abort();
  }
  if (_list == nullptr) {
    _list = new std::vector<SmartyMqttSubscription*>();
  }
  _list->push_back(this);
  _listIndex = _list->size() - 1;
}

SmartyMqttSubscription::~SmartyMqttSubscription() {
  _list->erase(_list->begin() + _listIndex);
}

const char* SmartyMqttSubscription::getTopic() {
  return _topic;
}

void SmartyMqttSubscription::setCallback(SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE callback) {
  _callback = callback;
}

bool SmartyMqttSubscription::canHandle(const char *topic) {
  // TODO: Use regular expression to compare topics
  return strcmp(_topic, topic) == 0;
}

void SmartyMqttSubscription::handle(const char *topic, const char* message) {
  if (_callback != nullptr) {
    _callback(topic, message);
  }
}
