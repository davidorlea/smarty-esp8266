#include "Smarty.hpp"

Smarty::Smarty()
: _config()
, _firmware()
, _uptime()
, _wifi()
, _ota()
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
  if (_config.getWifiSSID()) {
    _wifi.setSSID(_config.getWifiSSID());
  }
  if (_config.getWifiPassword()) {
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
  if (_config.getMqttHost()) {
    _mqtt.setHost(_config.getMqttHost());
  }
  if (_config.getMqttPort()) {
    _mqtt.setPort(_config.getMqttPort());
  }
  if (_config.getMqttClientId()) {
    _mqtt.setClientId(_config.getMqttClientId());
  }
  if (_config.getMqttSystemTopic()) {
    _mqtt.setSystemTopic(_config.getMqttSystemTopic());
  }
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
    _ota.loop();
    _http.loop();
    _mqtt.loop();
  }
}
