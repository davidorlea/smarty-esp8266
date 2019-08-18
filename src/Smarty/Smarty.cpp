#include "Smarty.hpp"

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

  Serial << "Initializing System: ";
  _initializeSystem();
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
  _initializeWifi();
  _wifi.setup();
  Serial << "Done" << endl;

  Serial << "Initializing OTA Update server: ";
  _ota.setup();
  Serial << "Done" << endl;

  Serial << "Initializing HTTP server: ";
  _initializeHttp();
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

void Smarty::_initializeSystem() {
  if (_config.getName()[0]) {
    _name = _config.getName();
  }
}

void Smarty::_initializeWifi() {
  if (_config.getWifiSSID()[0]) {
    _wifi.setSSID(_config.getWifiSSID());
  }
  if (_config.getWifiPassword()[0]) {
    _wifi.setPassword(_config.getWifiPassword());
  }
  if (_config.getWifiHostname()[0]) {
    _wifi.setHostname(_config.getWifiHostname());
  }
}

void Smarty::_initializeHttp() {
  _http.addCustomRoute("/api/v1/system", HTTP_GET, [this]() {
    // Extending buffer space (128 bytes) for String objects. See comment below.
    StaticJsonBuffer<JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 128> jsonBuffer;
    JsonObject& json = _createSystemJson(jsonBuffer);
    _http.sendSuccessResponse(json);
  });
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    _http.addCustomRoute("/api/v1/actuator/", actuator->getName(), HTTP_GET, [this, actuator]() {
      StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> jsonBuffer;
      JsonObject& json = actuator->toJson(jsonBuffer);
      _http.sendSuccessResponse(json);
    });
    _http.addCustomRoute("/api/v1/actuator/", actuator->getName(), HTTP_POST, [this, actuator]() {
      int state = _http.extractStateFromJson();
      if (actuator->parseState(state)) {
        StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> jsonBuffer;
        JsonObject& json = actuator->toJson(jsonBuffer);
        _http.sendSuccessResponse(json);
      } else {
        _http.sendErrorResponse(SmartyHttp::Error::BAD_REQUEST);
      }
    });
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    _http.addCustomRoute("/api/v1/sensor/", sensor->getName(), HTTP_GET, [this, sensor]() {
      StaticJsonBuffer<SmartyAbstractSensor::JSON_SIZE> jsonBuffer;
      JsonObject& json = sensor->toJson(jsonBuffer);
      _http.sendSuccessResponse(json);
    });
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
    sensor->addStateCallback([this, sensor]() {
      _mqtt.publish(sensor->getName(), "2");
    });
  }

  auto * timer = new SmartyTimer(SmartyMqtt::MQTT_STATUS_INTERVAL);
  timer->setCallback([this]() {
    // Extending buffer space (128 bytes) for String objects. See comment below.
    StaticJsonBuffer<JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 128> jsonBuffer;
    JsonObject& json = _createSystemJson(jsonBuffer);
    _mqtt.publishJson("$system", json);
  });
  timer->setCondition([this]() {
     return _mqtt.isConnected();
  });
}

// Debug JSON buffer sizes with:
// Serial << "Usage of StaticJsonBuffer is " << jsonBuffer.size() << " out of " << jsonBuffer.capacity() << endl;
// Serial << "Length of JsonObject is " << json.measureLength() + 1 << endl;
//
// WARNING 1: if you pass a String or a const char* to parseArray() or parseObject, the JsonBuffer will make a copy of
// the input, so it will need to be significantly bigger.
//
// WARNING 2: if you use String to create your JSON keys or values, their content will automatically be duplicated in
// the JsonBuffer, so you need to add the total length of all strings in the size of the JsonBuffer.

JsonObject& Smarty::_createSystemJson(JsonBuffer& jsonBuffer) {
  JsonObject& rootJson = jsonBuffer.createObject();
  if (_name) {
    rootJson["name"] = _name;
  }
  rootJson["uptime"] = _uptime.getSeconds();
  JsonObject& firmwareJson = rootJson.createNestedObject("firmware");
  firmwareJson["name"] = _firmware.name;
  firmwareJson["version"] = _firmware.version;
  JsonObject& wifiJson = rootJson.createNestedObject("wifi");
  wifiJson["ssid"] = _wifi.getSSID();
  wifiJson["rssi"] = _wifi.getRSSI();
  wifiJson["ip"] = _wifi.getIpAddress();
  wifiJson["hostname"] = _wifi.getHostName();
  return rootJson;
}
