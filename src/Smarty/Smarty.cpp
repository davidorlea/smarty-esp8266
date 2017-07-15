#include "Smarty.hpp"

Smarty::Smarty()
: _config()
, _firmware()
, _uptime()
, _wifi()
, _ota()
, _http(_firmware, _uptime, _wifi)
, _mqtt() {
}

Smarty::~Smarty() {
  Serial << "ERROR: Destruction of Smarty not allowed!" << endl;
  abort();
}

void Smarty::setFirmwareName(const char* firmwareName) {
  _firmware.name = firmwareName;
}

void Smarty::setFirmwareVersion(const char* firmwareVersion) {
  _firmware.version = firmwareVersion;
}

void Smarty::setup() {
  Serial << "Starting Smarty setup ..." << endl;

  Serial << "Initializing Configuration: ";
  _config.setup();
  Serial << "Done" << endl;

  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    Serial << "Initializing Actuator " << actuator->getName() << ": ";
    actuator->setup();
    Serial << "Done" << endl;
  }

  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    Serial << "Initializing Sensor " << sensor->getName() << ": ";
    sensor->setup();
    Serial << "Done" << endl;
  }

  Serial << "Initializing Wifi client: ";
  if (_config.getWifiSSID()[0]) {
    _wifi.setSSID(_config.getWifiSSID());
  }
  if (_config.getWifiPassword()[0]) {
    _wifi.setPassword(_config.getWifiPassword());
  }
  _wifi.setup();
  Serial << "Done" << endl;

  Serial << "Initializing OTA Update server: ";
  _ota.setup();
  Serial << "Done" << endl;

  Serial << "Initializing HTTP server: ";
  _http.setup();
  Serial << "Done" << endl;

  Serial << "Initializing MQTT client: ";
  _initializeMqtt();
  _mqtt.setup();
  Serial << "Done" << endl;

  Serial << "... Finished Smarty setup" << endl;
}

void Smarty::loop() {
  _uptime.update();
  for (SmartyTimer* timer : *SmartyTimer::getList()) {
    timer->update();
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    sensor->loop();
  }
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    actuator->loop();
  }
  if (_wifi.loop()) {
    _ota.loop();
    _http.loop();
    _mqtt.loop();
  }
}

void Smarty::_initializeMqtt() {
  if (_config.getMqttHost()[0]) {
    _mqtt.setHost(_config.getMqttHost());
  }
  if (_config.getMqttPort()) {
    _mqtt.setPort(_config.getMqttPort());
  }
  if (_config.getMqttUsername()[0]) {
    _mqtt.setUsername(_config.getMqttUsername());
  }
  if (_config.getMqttPassword()[0]) {
    _mqtt.setPassword(_config.getMqttPassword());
  }
  if (_config.getMqttClientId()[0]) {
    _mqtt.setClientId(_config.getMqttClientId());
  }
  if (_config.getMqttBaseTopic()[0]) {
    _mqtt.setBaseTopic(_config.getMqttBaseTopic());
  }

  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    actuator->addActivateCallback([this, actuator](bool changed) {
        _mqtt.publish(actuator->getName(), "1");
    });
    actuator->addDeactivateCallback([this, actuator](bool changed) {
        _mqtt.publish(actuator->getName(), "0");
    });
    _mqtt.subscribe(actuator->getName(), "/set", [actuator](const char* topic, const char* message) {
        if (strcmp(message, "0") == 0) {
          actuator->deactivate();
        } else if (strcmp(message, "1") == 0) {
          actuator->activate();
        } else if (strcmp(message, "2") == 0) {
          actuator->toggle();
        }
    });
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    sensor->addStateCallback([this, sensor](uint8_t state) {
        _mqtt.publish(sensor->getName(), "2");
    });
  }

  SmartyTimer* timer = new SmartyTimer(SMARTY_MQTT_STATUS_INTERVAL);
  timer->setCallback([this]() {
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

      _mqtt.publishJson("$system", root);
  });
  timer->setCondition([this]() {
     return _mqtt.isConnected();
  });
}
