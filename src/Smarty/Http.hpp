#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

class SmartyHttp {
public:
  SmartyHttp();
  void setup();
  void loop();
  void addCustomRoute(const char*, const char*, HTTPMethod, std::function<void (void)>);
  void addCustomRoute(const char*, HTTPMethod, std::function<void (void)>);
  void handleOk(JsonObject &json);
  void handleBadRequest();
  void handleNotFound();
  int extractStateFromJson();
private:
  ESP8266WebServer _webServer;
  void _sendJson(int, JsonObject&);
};
