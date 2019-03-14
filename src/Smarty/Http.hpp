#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

class SmartyHttp {
public:
  enum class Error {
    BAD_REQUEST,
    NOT_FOUND,
    INTERNAL_SERVER_ERROR
  };
  void setup();
  void loop();
  void addCustomRoute(const char*, const char*, HTTPMethod, std::function<void (void)>);
  void addCustomRoute(const char*, HTTPMethod, std::function<void (void)>);
  void sendSuccessResponse(JsonObject&);
  void sendErrorResponse(Error = Error::INTERNAL_SERVER_ERROR);
  int extractStateFromJson();
private:
  ESP8266WebServer _webServer{80};
};
