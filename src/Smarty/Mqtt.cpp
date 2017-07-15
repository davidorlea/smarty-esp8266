#include "Mqtt.hpp"

SmartyMqtt::SmartyMqtt(SmartyFirmware& firmware, SmartyUptime& uptime, SmartyWifi& wifi)
: _wifiClient()
, _pubSubClient(_wifiClient)
, _firmware(firmware)
, _uptime(uptime)
, _wifi(wifi)
, _lastConnectionAttempt(0)
, _lastStatusPublish(0) {
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
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    _addCustomPublication(actuator);
    _addCustomSubscription(actuator);
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    _addCustomPublication(sensor);
  }
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
    if (_lastStatusPublish == 0 || now - _lastStatusPublish >= SMARTY_MQTT_STATUS_INTERVAL) {
      _lastStatusPublish = now;
      _publishSystem();
    }
    for (SmartyMqttPublication* publication: *SmartyMqttPublication::getList()) {
      if (publication->isReady()) {
        _publish(publication->getTopic(), publication->getMessage(), publication->getRetain());
        publication->ready(false);
      }
    }
  }
  _pubSubClient.loop();
}

void SmartyMqtt::_addCustomPublication(SmartyAbstractActuator* actuator) {
  actuator->addActivateCallback([this, actuator](bool changed) {
    _publish(actuator->getName(), "1");
  });
  actuator->addDeactivateCallback([this, actuator](bool changed) {
    _publish(actuator->getName(), "0");
  });
};

void SmartyMqtt::_addCustomPublication(SmartyAbstractSensor* sensor) {
  sensor->addStateCallback([this, sensor](uint8_t state) {
    _publish(sensor->getName(), "2");
  });
};

void SmartyMqtt::_addCustomSubscription(SmartyAbstractActuator* actuator) {
  char topic[strlen(_baseTopic) + 1 + strlen(actuator->getName()) + 4 + 1];
  strcpy(topic, _baseTopic);
  strcat(topic, "/");
  strcat(topic, actuator->getName());
  strcat(topic, "/set");

  SmartyMqttSubscription* subscription = new SmartyMqttSubscription(topic);
  subscription->setCallback([actuator](const char* topic, const char* message) {
    if (strcmp(message, "0") == 0) {
      actuator->deactivate();
    } else if (strcmp(message, "1") == 0) {
      actuator->activate();
    } else if (strcmp(message, "2") == 0) {
      actuator->toggle();
    }
  });

  _subscriptions.push_back(subscription);
}

void SmartyMqtt::_connect() {
  if (!_pubSubClient.connected()) {
    Serial << "(Re-)Connecting to MQTT broker ..." << endl;
    Serial << "MQTT Broker: " << _host << ":" << _port << endl;
    Serial << "MQTT Client ID: " << _clientId << endl;
    Serial << "MQTT Username: " << ((_username) ? _username : "n/a") << endl;
    Serial << "MQTT Password: " << ((_password) ? _password : "n/a") << endl;

    if (_username && _password) {
      _pubSubClient.connect(_clientId, _username, _password, _baseTopic, 0, false, "{\"message\":\"good bye\"}");
    } else {
      _pubSubClient.connect(_clientId, _baseTopic, 0, false, "{\"message\":\"good bye\"}");
    }

    if (_pubSubClient.connected()) {
      Serial << "... successfully (re-)connected to MQTT broker" << endl;
    } else {
      Serial << "... failed (re-)connecting to MQTT broker (" << _pubSubClient.state() << ")" << endl;
      return;
    }

    for (SmartyMqttSubscription* subscription : *SmartyMqttSubscription::getList()) {
      _pubSubClient.subscribe(subscription->getTopic());
    }
    _publish(_baseTopic, "{\"message\":\"hello world\"}");
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

void SmartyMqtt::_publishSystem() {
  StaticJsonBuffer<JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["uptime"] = _uptime.getSeconds();
  JsonObject& firmware = root.createNestedObject("firmware");
  firmware["name"] = _firmware.name;
  firmware["version"] = _firmware.version;
  JsonObject& wifi = root.createNestedObject("wifi");
  wifi["ssid"] = _wifi.getSSID();
  wifi["rssi"] = _wifi.getRSSI();
  wifi["ip"] = _wifi.getIpAddress();
  wifi["hostname"] = _wifi.getHostName();

  const char suffix[] = "/system";
  _publishJson(suffix, root);
}

void SmartyMqtt::_publishJson(const char* topic, JsonObject& json, bool retain) {
  size_t jsonLength = json.measureLength() + 1;
  char payload[jsonLength];
  json.printTo(payload, jsonLength);

  _publish(topic, payload, retain);
}

void SmartyMqtt::_publish(const char* topic, const char* payload, bool retain) {
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
