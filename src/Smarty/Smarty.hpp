#pragma once

#include <Arduino.h>
#include <Streaming.h>
#include "Firmware.hpp"
#include "Http.hpp"
#include "Mqtt.hpp"
#include "Ota.hpp"
#include "Transducer/AbstractActuator.hpp"
#include "Transducer/AbstractSensor.hpp"
#include "Transducer/Button.hpp"
#include "Transducer/Led.hpp"
#include "Transducer/Relay.hpp"
#include "Uptime.hpp"
#include "Wifi.hpp"

class Smarty {
public:
  Smarty();
  ~Smarty();
  void setFirmwareName(const char*);
  void setFirmwareVersion(const char*);
  void setWifiSSID(const char*);
  void setWifiPassword(const char*);
  void setMqttServer(const char*, const uint16_t);
  void setMqttClientId(const char*);
  void setMqttSystemTopic(const char*);
  void setup();
  void loop();
private:
  SmartyFirmware _firmware;
  SmartyUptime _uptime;
  SmartyWifi _wifi;
  SmartyOta _ota;
  SmartyHttp _http;
  SmartyMqtt _mqtt;
};
