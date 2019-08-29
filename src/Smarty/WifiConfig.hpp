#pragma once

class SmartyWifiConfig {
public:
  virtual const char* getWifiSSID() const = 0;
  virtual const char* getWifiPassword() const = 0;
  virtual const char* getWifiHostname() const = 0;
};
