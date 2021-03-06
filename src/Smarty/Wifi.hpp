#pragma once

#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <IPAddress.h>
#include <Streaming.h>
#include "AbstractJsonSerializable.hpp"
#include "WifiConfig.hpp"

extern "C" {
  #include "user_interface.h"
}

class SmartyWifi : public SmartyAbstractJsonSerializable {
public:
  using SmartyAbstractJsonSerializable::toJson;
  static const unsigned int JSON_SIZE = JSON_OBJECT_SIZE(4) + 128;
  SmartyWifi();
  String getSSID();
  int32_t getRSSI();
  String getIpAddress();
  String getHostName();
  void setup(SmartyWifiConfig&);
  bool loop();
  JsonObject& toJson(JsonObject&) override;
private:
  const char* _ssid = nullptr;
  const char* _password = nullptr;
  const char* _hostname = nullptr;
  bool _isWaitingForConnection = false;
  bool _connect();
  void _clearDefaultSettings();
  void _clearCurrentSettings();
  void _printCurrentAndDefaultSettings();
  void _printNetworkData();
  bool _isConnected();
};
