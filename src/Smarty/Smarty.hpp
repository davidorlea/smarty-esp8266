#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Streaming.h>
#include "Config.hpp"
#include "Firmware.hpp"
#include "Http.hpp"
#include "Mqtt.hpp"
#include "Ota.hpp"
#include "Timer.hpp"
#include "Transducer/AbstractActuator.hpp"
#include "Transducer/AbstractSensor.hpp"
#include "Transducer/Button.hpp"
#include "Transducer/Led.hpp"
#include "Transducer/Relay.hpp"
#include "Uptime.hpp"
#include "Wifi.hpp"

class Smarty {
public:
  ~Smarty();
  void setFirmwareName(const char*);
  void setFirmwareVersion(const char*);
  void setup();
  void loop();
private:
  SmartyConfig _config{};
  SmartyFirmware _firmware{};
  SmartyUptime _uptime{};
  SmartyWifi _wifi{};
  SmartyOta _ota{};
  SmartyHttp _http{};
  SmartyMqtt _mqtt{};
  const char* _name = nullptr;
  void _initializeSystem();
  void _initializeWifi();
  void _initializeHttp();
  void _initializeMqtt();
  JsonObject& _createSystemJson(JsonBuffer&);
  JsonObject& _createTransducerJson(JsonBuffer&, SmartyAbstractTransducer*);
};
