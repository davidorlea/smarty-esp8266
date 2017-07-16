#include "Http.hpp"

SmartyHttp::SmartyHttp()
: _webServer(80) {
}

void SmartyHttp::setup() {
  _webServer.onNotFound([this]() {
    return handleNotFound();
  });
  _webServer.begin();
}

void SmartyHttp::loop() {
  _webServer.handleClient();
}

void SmartyHttp::addCustomRoute(const char* uriPrefix, const char* uri, HTTPMethod method, std::function<void (void)> handler) {
  char composedUri[strlen(uriPrefix) + strlen(uri) + 1];
  strcpy(composedUri, uriPrefix);
  strcat(composedUri, uri);
  addCustomRoute(composedUri, method, handler);
}

void SmartyHttp::addCustomRoute(const char* uri, HTTPMethod method, std::function<void (void)> handler) {
  _webServer.on(uri, method, handler);
}

void SmartyHttp::handleOk(JsonObject& json) {
  _sendJson(200, json);
}

void SmartyHttp::handleBadRequest() {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 18> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["error"] = "BAD_REQUEST";
  _sendJson(400, root);
}

void SmartyHttp::handleNotFound() {
  StaticJsonBuffer<JSON_OBJECT_SIZE(1) + 16> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["error"] = "NOT_FOUND";
  _sendJson(404, root);
}

int SmartyHttp::extractStateFromJson() {
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

void SmartyHttp::_sendJson(int code, JsonObject& json) {
  size_t jsonLength = json.measureLength() + 1;
  char payload[jsonLength];
  json.printTo(payload, jsonLength);
  _webServer.send(code, "application/json", payload);
}
