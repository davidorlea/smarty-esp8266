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

void SmartyMqtt::setHost(const char* host) {
  _host = host;
}

void SmartyMqtt::setPort(const uint16_t port) {
  _port = port;
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
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    _addCustomPublication(actuator);
    _addCustomSubscription(actuator);
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    _addCustomPublication(sensor);
  }
}

void SmartyMqtt::loop() {
  unsigned long now = millis();

  if (!_pubSubClient.connected()
      && (_lastConnectionAttempt == 0 || now - _lastConnectionAttempt >= MQTT_RECONNECT_INTERVAL)) {
    _lastConnectionAttempt = now;
    _connect();
  }
  _pubSubClient.loop();

  if (_pubSubClient.connected()) {
    if (_lastStatusPublish == 0 || now - _lastStatusPublish >= MQTT_STATUS_INTERVAL) {
      _lastStatusPublish = now;
      _publishSystem();
    }
    for (SmartyMqttPublication* publication: *SmartyMqttPublication::getList()) {
      if (publication->isReady()) {
        _publish(publication->getTopic(), publication->getMessage());
        publication->ready(false);
      }
    }
  }
}

void SmartyMqtt::_addCustomPublication(SmartyAbstractActuator* actuator) {
  char topic[strlen(_baseTopic) + 1 + strlen(actuator->getName()) + 1];
  strcpy(topic, _baseTopic);
  strcat(topic, "/");
  strcat(topic, actuator->getName());

  // FIXME: Memory leak
  SmartyMqttPublication* publication = new SmartyMqttPublication(topic);
  actuator->addActivateCallback([publication](bool changed) {
      publication->setMessage("1");
      publication->ready();
  });
  actuator->addDeactivateCallback([publication](bool changed) {
      publication->setMessage("0");
      publication->ready();
  });

};

void SmartyMqtt::_addCustomPublication(SmartyAbstractSensor* sensor) {
  char topic[strlen(_baseTopic) + 1 + strlen(sensor->getName()) + 1];
  strcpy(topic, _baseTopic);
  strcat(topic, "/");
  strcat(topic, sensor->getName());

  // FIXME: Memory leak
  SmartyMqttPublication* publication = new SmartyMqttPublication(topic);
  sensor->addStateCallback([publication](uint8_t state) {
      publication->setMessage("2");
      publication->ready();
  });
};

void SmartyMqtt::_addCustomSubscription(SmartyAbstractActuator* actuator) {
  char topic[strlen(_baseTopic) + 1 + strlen(actuator->getName()) + 4 + 1];
  strcpy(topic, _baseTopic);
  strcat(topic, "/");
  strcat(topic, actuator->getName());
  strcat(topic, "/set");

  // FIXME: Memory leak
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
}

void SmartyMqtt::_connect() {
  if (!_pubSubClient.connected()) {
    Serial << "(Re-)Connecting to MQTT broker: ";

    if (_pubSubClient.connect(_clientId, _baseTopic, 0, false, "{\"message\":\"good bye\"}")) {
      Serial << "Done" << endl;
      Serial << "MQTT Broker: " << _host << ":" << _port << endl;
      Serial << "MQTT Client ID: " << _clientId << endl;
    } else {
      Serial << "Failed (" << _pubSubClient.state() << ")" << endl;
      return;
    }

    for (SmartyMqttSubscription* subscription : *SmartyMqttSubscription::getList()) {
      _pubSubClient.subscribe(subscription->getTopic());
    }
    _publish(_baseTopic, "{\"message\":\"hello world\"}");
  }
}

void SmartyMqtt::_callback(char* topic, byte* payload, unsigned int length) {
  char message[length+1];
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
  char systemTopic[strlen(_baseTopic) + strlen(suffix) + 1];
  strcpy(systemTopic, _baseTopic);
  strcat(systemTopic, suffix);

  _publishJson(systemTopic, root);
}

void SmartyMqtt::_publishJson(const char* topic, JsonObject& json) {
  size_t jsonLength = json.measureLength() + 1;
  char payload[jsonLength];
  json.printTo(payload, jsonLength);

  _publish(topic, payload);
}

void SmartyMqtt::_publish(const char* topic, const char* payload) {
  if (_pubSubClient.connected() && _pubSubClient.publish(topic, payload)) {
    Serial << "Outgoing MQTT message [";
  } else {
    Serial << "Discarded outgoing MQTT message [";
  }
  Serial << topic << "]: " << payload << endl;
}
