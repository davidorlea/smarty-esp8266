#include "Mqtt.hpp"

SmartyMqtt::SmartyMqtt(SmartyFirmware& firmware, SmartyUptime& uptime, SmartyWifi& wifi)
: _wifiClient()
, _pubSubClient(_wifiClient)
, _firmware(firmware)
, _uptime(uptime)
, _wifi(wifi)
, _lastConnectionAttempt(0)
, _lastStatusPublish(0) {
  uint8_t mac[6];
  _wifi.getMacAddress(mac);
  // FIXME: Find better solution to prevent memory leak
  char* composedClientId = (char*) malloc(21 + 1);
  sprintf(composedClientId, "smarty-esp8266-%02X%02X%02X", mac[3], mac[4], mac[5]);
  _clientId = composedClientId;
}

void SmartyMqtt::setServer(const char* serverUrl, const uint16_t serverPort) {
  _serverUrl = serverUrl;
  _serverPort = serverPort;
}

void SmartyMqtt::setClientId(const char* clientId) {
  _clientId = clientId;
}

void SmartyMqtt::setSystemTopic(const char* systemTopic) {
  _systemTopic = systemTopic;
}

void SmartyMqtt::setup() {
  _pubSubClient.setServer(_serverUrl, _serverPort);
  _pubSubClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
    return _callback(topic, payload,length);
  });
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

void SmartyMqtt::_connect() {
  if (!_pubSubClient.connected()) {
    Serial << "(Re-)Connecting to MQTT broker: ";

    if (_pubSubClient.connect(_clientId, _systemTopic, 0, false, "{\"message\":\"good bye\"}")) {
      Serial << "Done" << endl;
      Serial << "MQTT Broker: " << _serverUrl << ":" << _serverPort << endl;
      Serial << "MQTT Client ID: " << _clientId << endl;
    } else {
      Serial << "Failed (" << _pubSubClient.state() << ")" << endl;
      return;
    }

    for (SmartyMqttSubscription* subscription : *SmartyMqttSubscription::getList()) {
      _pubSubClient.subscribe(subscription->getTopic());
    }
    _publish(_systemTopic, "{\"message\":\"hello world\"}");
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
  StaticJsonBuffer<JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 128> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["uptime"] = _uptime.getSeconds();
  JsonObject& firmware = root.createNestedObject("firmware");
  firmware["name"] = _firmware.name;
  firmware["version"] = _firmware.version;
  JsonObject& wifi = root.createNestedObject("wifi");
  wifi["ssid"] = _wifi.getSSID();
  wifi["rssi"] = _wifi.getRSSI();
  wifi["ip"] = _wifi.getIpAddress();

  _publishJson(_systemTopic, root);
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
