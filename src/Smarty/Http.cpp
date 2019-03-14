#include "Http.hpp"

void SmartyHttp::setup() {
  _webServer.onNotFound([this]() {
    sendErrorResponse(Error::NOT_FOUND);
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

void SmartyHttp::sendSuccessResponse(JsonObject& json) {
  size_t jsonLength = json.measureLength() + 1;
  char payload[jsonLength];
  json.printTo(payload, jsonLength);
  _webServer.send(200, "application/json", payload);
}

void SmartyHttp::sendErrorResponse(Error error) {
  switch(error) {
    case Error::BAD_REQUEST:
      _webServer.send(400, "application/json", R"({"error":"BAD_REQUEST"})");
      break;
    case Error::NOT_FOUND:
      _webServer.send(404, "application/json", R"({"error":"NOT_FOUND"})");
      break;
    case Error::INTERNAL_SERVER_ERROR:
      _webServer.send(500, "application/json", R"({"error":"INTERNAL_SERVER_ERROR"})");
      break;
  }
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
