#include "Mqtt.hpp"

SmartyMqtt::SmartyMqtt()
: _mqttClient(this, 384) {
  auto* composedClientId = (char*) malloc(21 + 1);
  sprintf(composedClientId, "smarty-esp8266-%06x", ESP.getChipId());
  _clientId = composedClientId;

  auto* composedBaseTopic = (char*) malloc(21 + 1);
  sprintf(composedBaseTopic, "smarty/esp8266-%06x", ESP.getChipId());
  _baseTopic = composedBaseTopic;
}

SmartyMqtt::~SmartyMqtt() {
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
  char topic[] = "/$online";
  char composedTopic[strlen(_baseTopic) + strlen(topic) + 1];
  strcpy(composedTopic, _baseTopic);
  strcat(composedTopic, topic);

  _mqttClient.begin(_host, _port, _wifiClient);
  _mqttClient.setWill(composedTopic, "false", (boolean) true, 0);
  _mqttClient.onMessageAdvanced([](MQTTClient* client, char* topic, char *bytes, int length) {
    auto* parent = static_cast<SmartyMqtt*>(static_cast<SmartyMQTTClient*>(client)->parent); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
    parent->_callback(topic, bytes, length);
  });
}

void SmartyMqtt::loop() {
  unsigned long now = millis();
  if (!_mqttClient.connected()
      && (_lastConnectionAttempt == 0 || now - _lastConnectionAttempt >= MQTT_RECONNECT_INTERVAL)) {
    _lastConnectionAttempt = now;
    _connect();
  }
  _mqttClient.loop();
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

  bool result = _mqttClient.publish(composedTopic, payload, (boolean) retain, 0);
  if (result) {
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

  auto* subscription = new SmartyMqttSubscription(composedTopic);
  subscription->setCallback(callback);
  _subscriptions.push_back(subscription);
}

bool SmartyMqtt::isConnected() {
  return _mqttClient.connected();
}

void SmartyMqtt::_connect() {
  if (!_mqttClient.connected()) {
    Serial << "(Re-)Connecting to MQTT broker ..." << endl;
    Serial << "MQTT Broker: " << _host << ":" << _port << endl;
    Serial << "MQTT Client ID: " << _clientId << endl;
    Serial << "MQTT Username: " << ((_username) ? _username : "n/a") << endl;
    Serial << "MQTT Password: " << ((_password) ? _password : "n/a") << endl;

    if (_username && _password) {
      _mqttClient.connect(_clientId, _username, _password);
    } else {
      _mqttClient.connect(_clientId);
    }

    if (_mqttClient.connected()) {
      Serial << "... successfully (re-)connected to MQTT broker" << endl;
    } else {
      Serial << "... failed (re-)connecting to MQTT broker" << endl;
      return;
    }
    publish("$online", "true", true);

    for (SmartyMqttSubscription* subscription : _subscriptions) {
      _mqttClient.subscribe(subscription->getTopic(), 0);
    }
  }
}

void SmartyMqtt::_callback(char* topic, char* bytes, int length) {
  char message[length + 1];
  memcpy(message, bytes, length);
  message[length] = '\0';

  Serial << "Incoming MQTT message [" << topic << "]: " << message << endl;

  for (SmartyMqttSubscription* subscription : _subscriptions) {
    if (subscription->canHandle(topic)) {
      subscription->handle(topic, message);
    }
  }
}
