#include "Http.hpp"

SmartyHttp::SmartyHttp(SmartyFirmware& firmware, SmartyUptime& uptime, SmartyWifi& wifi)
: _webServer(80)
, _firmware(firmware)
, _uptime(uptime)
, _wifi(wifi) {
}

void SmartyHttp::setup() {
  _addCustomRoute("/api/v1/system", HTTP_GET, [this]() {
    return _handleGetSystem();
  });
  for (SmartyAbstractActuator* actuator : *SmartyAbstractActuator::getList()) {
    _addCustomRoute("/api/v1/actuator/", actuator->getName(), HTTP_GET, [this, actuator]() {
      return _handleGetNode(actuator);
    });
    _addCustomRoute("/api/v1/actuator/", actuator->getName(), HTTP_POST, [this, actuator]() {
      return _handlePostActuator(actuator);
    });
  }
  for (SmartyAbstractSensor* sensor : *SmartyAbstractSensor::getList()) {
    _addCustomRoute("/api/v1/sensor/", sensor->getName(), HTTP_GET, [this, sensor]() {
      return _handleGetNode(sensor);
    });
  }
  _webServer.onNotFound([this]() {
    return _handleNotFound();
  });
  _webServer.begin();
}

void SmartyHttp::loop() {
  _webServer.handleClient();
}

void SmartyHttp::_addCustomRoute(const char* uri, HTTPMethod method, std::function<void (void)> handler) {
  _addCustomRoute("", uri, method, handler);
}

void SmartyHttp::_addCustomRoute(const char* uriPrefix, const char* uri, HTTPMethod method, std::function<void (void)> handler) {
  char composedUri[strlen(uriPrefix) + strlen(uri) + 1];
  strcpy(composedUri, uriPrefix);
  strcat(composedUri, uri);
  _webServer.on(composedUri, method, handler);
}

void SmartyHttp::_handleGetSystem() {
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

  _sendJson(200, root);
}

void SmartyHttp::_handleGetNode(SmartyAbstractTransducer* node) {
  StaticJsonBuffer<JSON_OBJECT_SIZE(2) + 64> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = node->getName();
  root["state"] = node->state();
  _sendJson(200, root);
}

void SmartyHttp::_handlePostActuator(SmartyAbstractActuator* actuator) {
  int state = _extractStateFromJson();
  if (actuator->parseState(state)) {
    _handleGetNode(actuator);
  } else {
    _handleBadRequest();
  }
}

void SmartyHttp::_handleBadRequest() {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 18> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["error"] = "BAD_REQUEST";
  _sendJson(400, root);
}

void SmartyHttp::_handleNotFound() {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 16> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["error"] = "NOT_FOUND";
  _sendJson(404, root);
}

void SmartyHttp::_sendJson(int code, JsonObject& json) {
  size_t jsonLength = json.measureLength() + 1;
  char payload[jsonLength];
  json.printTo(payload, jsonLength);
  _webServer.send(code, "application/json", payload);
}

int SmartyHttp::_extractStateFromJson() {
  if (!_webServer.hasArg("plain")) {
    return -1;
  }

  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 16> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(_webServer.arg("plain").c_str());

  if (!root.success() || !root.containsKey("state")) {
    return -1;
  }

  return root["state"].as<int>();
}
