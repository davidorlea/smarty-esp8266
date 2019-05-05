#include "MqttClient.hpp"

SmartyMQTTClient::SmartyMQTTClient(void* parent, int bufSize)
: MQTTClient(bufSize)
, parent(parent) {
}
