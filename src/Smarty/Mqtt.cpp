#include "Mqtt.hpp"

SmartyMqtt::SmartyMqtt()
: _wifiClient()
, _pubSubClient(_wifiClient)
, _lastConnectionAttempt(0) {
  char* composedClientId = (char*) malloc(21 + 1);
  sprintf(composedClientId, "smarty-esp8266-%06x", ESP.getChipId());
  _clientId = composedClientId;

  char* composedBaseTopic = (char*) malloc(21 + 1);
  sprintf(composedBaseTopic, "smarty/esp8266-%06x", ESP.getChipId());
  _baseTopic = composedBaseTopic;
}

SmartyMqtt::~SmartyMqtt() {
  for (SmartyMqttPublication* publication: _publications) {
    delete publication;
  }
  for (SmartyMqttSubscription* subscription: _subscriptions) {
    delete subscription;
  }
}

void SmartyMqtt::setHost(const char* host) {
  _host = host;
}

void SmartyMqtt::setPort(const uint16_t port) {
  _port = port;
}

void SmartyMqtt::setUsername(const char* username) {
  _username = username;
}

void SmartyMqtt::setPassword(const char* password) {
  _password = password;
}

void SmartyMqtt::setClientId(const char* clientId) {
  free((void*) _clientId);
  _clientId = clientId;
}

void SmartyMqtt::setBaseTopic(const char* topic) {
  free((void*) _baseTopic);
  _baseTopic = topic;
}

void SmartyMqtt::setup() {
  _pubSubClient.setServer(_host, _port);
  _pubSubClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
      return _callback(topic, payload,length);
  });
}

void SmartyMqtt::loop() {
  unsigned long now = millis();

  if (!_pubSubClient.connected()
      && (_lastConnectionAttempt == 0 || now - _lastConnectionAttempt >= SMARTY_MQTT_RECONNECT_INTERVAL)) {
    _lastConnectionAttempt = now;
    _connect();
  }
  if (_pubSubClient.connected()) {
    for (SmartyMqttPublication* publication: *SmartyMqttPublication::getList()) {
      if (publication->isReady()) {
        publish(publication->getTopic(), publication->getMessage(), publication->getRetain());
        publication->ready(false);
      }
    }
  }
  _pubSubClient.loop();
}

void SmartyMqtt::publishJson(const char* topic, JsonObject& json, bool retain) {
  size_t jsonLength = json.measureLength() + 1;
  char payload[jsonLength];
  json.printTo(payload, jsonLength);

  publish(topic, payload, retain);
}

void SmartyMqtt::publish(const char* topic, const char* payload, bool retain) {
  char composedTopic[strlen(_baseTopic) + 1 + strlen(topic) + 1];
  strcpy(composedTopic, _baseTopic);
  strcat(composedTopic, "/");
  strcat(composedTopic, topic);

  if (_pubSubClient.connected() && _pubSubClient.publish(composedTopic, payload, retain)) {
    Serial << "Outgoing MQTT message [";
  } else {
    Serial << "Discarded outgoing MQTT message [";
  }
  Serial << composedTopic << "]: " << payload << endl;
}

void SmartyMqtt::subscribe(const char* topic, const char* topicSuffix, SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE callback) {
  char composedTopic[strlen(topic) + strlen(topicSuffix) + 1];
  strcpy(composedTopic, topic);
  strcat(composedTopic, topicSuffix);

  subscribe(composedTopic, callback);
}

void SmartyMqtt::subscribe(const char* topic, SMARTY_MQTT_SUBSCRIPTION_CALLBACK_TYPE callback) {
  char composedTopic[strlen(_baseTopic) + 1 + strlen(topic) + 1];
  strcpy(composedTopic, _baseTopic);
  strcat(composedTopic, "/");
  strcat(composedTopic, topic);

  SmartyMqttSubscription* subscription = new SmartyMqttSubscription(composedTopic);
  subscription->setCallback(callback);

  _subscriptions.push_back(subscription);
}

bool SmartyMqtt::isConnected() {
  return _pubSubClient.connected();
}

void SmartyMqtt::_connect() {
  if (!_pubSubClient.connected()) {
    Serial << "(Re-)Connecting to MQTT broker ..." << endl;
    Serial << "MQTT Broker: " << _host << ":" << _port << endl;
    Serial << "MQTT Client ID: " << _clientId << endl;
    Serial << "MQTT Username: " << ((_username) ? _username : "n/a") << endl;
    Serial << "MQTT Password: " << ((_password) ? _password : "n/a") << endl;

    char topic[] = "/$online";
    char composedTopic[strlen(_baseTopic) + strlen(topic) + 1];
    strcpy(composedTopic, _baseTopic);
    strcat(composedTopic, topic);

    if (_username && _password) {
      _pubSubClient.connect(_clientId, _username, _password, composedTopic, 0, false, "false");
    } else {
      _pubSubClient.connect(_clientId, _baseTopic, 0, false, "{\"message\":\"good bye\"}");
    }

    if (_pubSubClient.connected()) {
      Serial << "... successfully (re-)connected to MQTT broker" << endl;
    } else {
      Serial << "... failed (re-)connecting to MQTT broker (" << _pubSubClient.state() << ")" << endl;
      return;
    }

    _pubSubClient.publish(composedTopic, "true");

    for (SmartyMqttSubscription* subscription : *SmartyMqttSubscription::getList()) {
      _pubSubClient.subscribe(subscription->getTopic());
    }
  }
}

void SmartyMqtt::_callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  Serial << "Incoming MQTT message [" << topic << "]: " << message << endl;

  for (SmartyMqttSubscription* subscription : *SmartyMqttSubscription::getList()) {
    if (subscription->canHandle(topic)) {
      subscription->handle(topic, message);
    }
  }
}
