#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <IPAddress.h>
#include <Streaming.h>

extern "C" {
  #include "user_interface.h"
}

class SmartyWifi {
public:
  void setSSID(const char*);
  void setPassword(const char*);
  String getSSID();
  int32_t getRSSI();
  String getIpAddress();
  String getHostName();
  void setup();
  bool loop();
private:
  const char* _ssid = nullptr;
  const char* _password = nullptr;
  bool _isWaitingForConnection = false;
  bool _connect();
  void _clearDefaultSettings();
  void _clearCurrentSettings();
  void _printCurrentAndDefaultSettings();
  void _printNetworkData();
  bool _isConnected();
};
