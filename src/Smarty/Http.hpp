#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include "Firmware.hpp"
#include "Transducer/AbstractActuator.hpp"
#include "Transducer/AbstractTransducer.hpp"
#include "Transducer/AbstractSensor.hpp"
#include "Uptime.hpp"
#include "Wifi.hpp"

class SmartyHttp {
public:
  SmartyHttp(SmartyFirmware&, SmartyUptime&, SmartyWifi&);
  void setup();
  void loop();
private:
  ESP8266WebServer _webServer;
  SmartyFirmware& _firmware;
  SmartyUptime& _uptime;
  SmartyWifi& _wifi;
  void _addCustomRoute(const char*, const char*, HTTPMethod, std::function<void (void)>);
  void _addCustomRoute(const char*, HTTPMethod, std::function<void (void)>);
  void _handleGetSystem();
  void _handleGetTransducer(SmartyAbstractTransducer*);
  void _handlePostActuator(SmartyAbstractActuator*);
  void _handleBadRequest();
  void _handleNotFound();
  void _sendJson(int, JsonObject&);
  int _extractStateFromJson();
};
