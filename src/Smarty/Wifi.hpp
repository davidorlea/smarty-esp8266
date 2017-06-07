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
  SmartyWifi();
  void setSSID(const char*);
  void setPassword(const char*);
  String getSSID();
  int32_t getRSSI();
  String getIpAddress();
  String getHostName();
  void setup();
  bool loop();
private:
  const char* _ssid;
  const char* _password;
  bool _isWaitingForConnection;
  bool _connect();
  void _clearDefaults();
  void _printDebugData();
  void _printNetworkData();
  bool _isConnected();
};
