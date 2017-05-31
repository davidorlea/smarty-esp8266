#include "Smarty.hpp"

Smarty::Smarty()
: _firmware()
, _uptime()
, _wifi()
, _http(_firmware, _uptime, _wifi)
, _mqtt(_firmware, _uptime, _wifi) {
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

void Smarty::setWifiSSID(const char* wifiSSID) {
  _wifi.setSSID(wifiSSID);
}

void Smarty::setWifiPassword(const char* wifiPassword) {
  _wifi.setPassword(wifiPassword);
}

void Smarty::setMqttServer(const char* serverUrl, const uint16_t serverPort) {
  _mqtt.setServer(serverUrl, serverPort);
}

void Smarty::setMqttClientId(const char* clientId) {
  _mqtt.setClientId(clientId);
}

void Smarty::setMqttSystemTopic(const char* systemTopic) {
  _mqtt.setSystemTopic(systemTopic);
}

void Smarty::setup() {
  Serial << "Starting Smarty setup ..." << endl;

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
  _wifi.setup();
  Serial << "Done" << endl;

  Serial << "Initializing HTTP server: ";
  _http.setup();
  Serial << "Done" << endl;

  Serial << "Initializing MQTT client: ";
  _mqtt.setup();
  Serial << "Done" << endl;

  Serial << "... Finished Smarty setup" << endl;
}

void Smarty::loop() {
  _uptime.update();
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    sensor->loop();
  }
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    actuator->loop();
  }
  if (_wifi.loop()) {
    _http.loop();
    _mqtt.loop();
  }
}
