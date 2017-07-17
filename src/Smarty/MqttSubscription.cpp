#include "MqttSubscription.hpp"

bool SmartyMqttSubscription::isValidTopic(const char *topic) {
  // Code from http://git.eclipse.org/c/mosquitto/org.eclipse.mosquitto.git/tree/lib/util_mosq.c
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

SmartyMqttSubscription::SmartyMqttSubscription(const char* topic) {
  if (!SmartyMqttSubscription::isValidTopic(topic)) {
    Serial << "ERROR: Invalid MQTT subscription topic!" << endl;
    abort();
  }
  _topic = strcpy(new char[strlen(topic) + 1], topic);
}

SmartyMqttSubscription::~SmartyMqttSubscription() {
  delete _topic;
}

const char* SmartyMqttSubscription::getTopic() {
  return _topic;
}

void SmartyMqttSubscription::setCallback(SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE callback) {
  _callback = callback;
}

bool SmartyMqttSubscription::canHandle(const char *topic) {
  // Code from http://git.eclipse.org/c/mosquitto/org.eclipse.mosquitto.git/tree/lib/util_mosq.c
  const char* sub = _topic;

  if (!sub || !topic) {
   return false;
  }

  int subLength = strlen(sub);
  int topicLength = strlen(topic);
  int subPosition = 0;
  int topicPosition = 0;

  while (subPosition < subLength && topicPosition < topicLength) {
    if (sub[subPosition] == topic[topicPosition]) {
      if (topicPosition == topicLength-1) {
        if (subPosition == subLength-3 && sub[subPosition+1] == '/' && sub[subPosition+2] == '#') {
          return true;
        }
      }
      subPosition++;
      topicPosition++;
      if (topicPosition == topicLength && subPosition == subLength) {
        return true;
      }
      if (topicPosition == topicLength && subPosition == subLength-1 && sub[subPosition] == '+') {
        return true;
      }
    } else {
      if (sub[subPosition] == '+') {
        subPosition++;
        while (topicPosition < topicLength && topic[topicPosition] != '/') {
          topicPosition++;
        }
        if (topicPosition == topicLength && subPosition == subLength) {
          return true;
        }
      } else if (sub[subPosition] == '#') {
        return subPosition == subLength - 1;
      } else {
        return false;
      }
    }
  }
  return !(topicPosition < topicLength || subPosition < subLength);
}

void SmartyMqttSubscription::handle(const char *topic, const char* message) {
  if (_callback != nullptr) {
    _callback(topic, message);
  }
}
