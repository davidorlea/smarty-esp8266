#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Streaming.h>
#include "Config.hpp"
#include "Firmware.hpp"
#include "Http.hpp"
#include "Mqtt.hpp"
#include "Ota.hpp"
#include "System.hpp"
#include "Timer.hpp"
#include "Transducer/AbstractActuator.hpp"
#include "Transducer/AbstractSensor.hpp"
#include "Transducer/BinaryActuator.hpp"
#include "Transducer/Button.hpp"
#include "Uptime.hpp"
#include "Wifi.hpp"

class Smarty {
public:
  ~Smarty();
  void setup();
  void loop();
private:
  SmartyConfig _config{};
  SmartySystem _system{};
  SmartyFirmware _firmware{};
  SmartyUptime _uptime{};
  SmartyWifi _wifi{};
  SmartyOta _ota{};
  SmartyHttp _http{};
  SmartyMqtt _mqtt{};
  void _initializeHttp();
  void _initializeMqtt();
  JsonObject& _createSystemJson(JsonBuffer&);
};
