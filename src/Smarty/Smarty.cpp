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

void Smarty::setFirmwareBuildTime(int firmwareBuildTime) {
  _firmware.buildTime = firmwareBuildTime;
}

void Smarty::setup() {
  Serial << "Starting Smarty setup ..." << endl;

  Serial << "Initializing Configuration: ";
  _config.setup();
  Serial << "Done" << endl;

  Serial << "Initializing System: ";
  _system.setup(_config);
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
  _wifi.setup(_config);
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
  _mqtt.setup(_config);
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

void Smarty::_initializeHttp() {
  _http.addCustomRoute("/api/v1/system", HTTP_GET, [this]() {
    // Extending buffer space (128 bytes) for String objects. See comment below.
    StaticJsonBuffer<JSON_OBJECT_SIZE(4) + SmartyUptime::JSON_SIZE + SmartyFirmware::JSON_SIZE + SmartyWifi::JSON_SIZE + 128> jsonBuffer;
    _http.sendSuccessResponse(_createSystemJson(jsonBuffer));
  });
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    _http.addCustomRoute("/api/v1/actuator/", actuator->getName(), HTTP_GET, [this, actuator]() {
      StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> jsonBuffer;
      _http.sendSuccessResponse(actuator->toJson(jsonBuffer));
    });
    _http.addCustomRoute("/api/v1/actuator/", actuator->getName(), HTTP_POST, [this, actuator]() {
      StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> requestJsonBuffer;
      if (actuator->fromJson(requestJsonBuffer, _http.getRequestBody())) {
        StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> responseJsonBuffer;
        _http.sendSuccessResponse(actuator->toJson(responseJsonBuffer));
      } else {
        _http.sendErrorResponse(SmartyHttp::Error::BAD_REQUEST);
      }
    });
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    _http.addCustomRoute("/api/v1/sensor/", sensor->getName(), HTTP_GET, [this, sensor]() {
      StaticJsonBuffer<SmartyAbstractSensor::JSON_SIZE> jsonBuffer;
      _http.sendSuccessResponse(sensor->toJson(jsonBuffer));
    });
  }
}

void Smarty::_initializeMqtt() {
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    actuator->addActivateCallback([this, actuator](bool changed) {
      StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> jsonBuffer;
      _mqtt.publishJson(actuator->getName(), actuator->toJson(jsonBuffer));
    });
    actuator->addDeactivateCallback([this, actuator](bool changed) {
      StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> jsonBuffer;
      _mqtt.publishJson(actuator->getName(), actuator->toJson(jsonBuffer));
    });
    _mqtt.subscribe(actuator->getName(), "/set", [this, actuator](const char* topic, const char* message) {
      StaticJsonBuffer<SmartyAbstractActuator::JSON_SIZE> jsonBuffer;
      actuator->fromJson(jsonBuffer, message);
    });
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    sensor->addStateCallback([this, sensor]() {
      StaticJsonBuffer<SmartyAbstractSensor::JSON_SIZE> jsonBuffer;
      _mqtt.publishJson(sensor->getName(), sensor->toJson(jsonBuffer));
    });
  }

  auto* timer = new SmartyTimer(SmartyMqtt::MQTT_STATUS_INTERVAL);
  timer->setCallback([this]() {
    // Extending buffer space (128 bytes) for String objects. See comment below.
    StaticJsonBuffer<JSON_OBJECT_SIZE(4) + SmartyUptime::JSON_SIZE + SmartyFirmware::JSON_SIZE + SmartyWifi::JSON_SIZE + 128> jsonBuffer;
    _mqtt.publishJson("$system", _createSystemJson(jsonBuffer));
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
  _system.toJson(rootJson);
  _uptime.toJson(rootJson);
  _firmware.toJson(rootJson);
  _wifi.toJson(rootJson);
  return rootJson;
}
