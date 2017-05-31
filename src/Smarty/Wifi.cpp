#include "Wifi.hpp"

SmartyWifi::SmartyWifi()
: _isWaitingForConnection(false) {
}

void SmartyWifi::setSSID(const char* ssid) {
  _ssid = ssid;
}

void SmartyWifi::setPassword(const char* password) {
  _password = password;
}

String SmartyWifi::getSSID() {
  return WiFi.SSID();
}

int32_t SmartyWifi::getRSSI() {
  return WiFi.RSSI();
}

String SmartyWifi::getIpAddress() {
  return WiFi.localIP().toString();
}

String SmartyWifi::getMacAddress() {
  return WiFi.macAddress();
}

void SmartyWifi::getMacAddress(uint8_t *mac) {
  WiFi.macAddress(mac);
}

String SmartyWifi::getHostName() {
  return WiFi.hostname();
}

void SmartyWifi::setup() {
  // _clearDefaults();
  WiFi.disconnect();
  WiFi.softAPdisconnect();
}

bool SmartyWifi::loop() {
  return _connect();
}

bool SmartyWifi::_connect() {
  if (!_isConnected()) {
    if (!_isWaitingForConnection) {
      Serial << "(Re-)Connecting to Wifi ..." << endl;
      _isWaitingForConnection = true;
      WiFi.persistent(false);
      WiFi.begin(_ssid, _password);
    }
    return false;
  } else {
    if (_isWaitingForConnection) {
      Serial << "... sucessfully (re-)connected to Wifi" << endl;
      _isWaitingForConnection = false;
      _printNetworkData();
    }
    return true;
  }
}

void SmartyWifi::_clearDefaults() {
  Serial << "Clearing default Wifi settings (saved in flash): ";

  // Access default settings (saved in flash) - and not only current settings
  WiFi.persistent(true);
  // Set default operating mode to none (OFF)
  WiFi.mode(WIFI_OFF);
  // Clear default station (STA) settings
  WiFi.disconnect(false);
  // Clear default access point (AP) settings
  WiFi.softAPdisconnect(false);

  Serial << "Done" << endl;
}

void SmartyWifi::_printDebugData() {
  struct station_config current_conf;
  struct station_config default_conf;
  wifi_station_get_config(&current_conf);
  wifi_station_get_config_default(&default_conf);

  const char* wifiModes[] = { "OFF", "STA", "AP", "STA+AP" };

  Serial << "Wifi current operating mode: " << wifiModes[wifi_get_opmode()] << endl;
  Serial << "Wifi default operating mode (saved in flash): " << wifiModes[wifi_get_opmode_default()] << endl;
  Serial << "Wifi Station current SSID: " << *current_conf.ssid << endl;
  Serial << "Wifi Station current password: " << *current_conf.password << endl;
  Serial << "Wifi Station default SSID (saved in flash): " << *default_conf.ssid << endl;
  Serial << "Wifi Station default password (saved in flash): " << *default_conf.password << endl;
  // Serial << "Further Wifi Diagnostics: " << endl;
  // WiFi.printDiag(Serial);
}

void SmartyWifi::_printNetworkData() {
  Serial << "Wifi SSID: " << WiFi.SSID() << endl;
  Serial << "Wifi RSSI: " << WiFi.RSSI() << endl;
  Serial << "Wifi IP address: " << WiFi.localIP() << endl;
  Serial << "Wifi MAC address: " << WiFi.macAddress() << endl;
  Serial << "Wifi Hostname: " << WiFi.hostname() << endl;
}

bool SmartyWifi::_isConnected() {
  return WiFi.status() == WL_CONNECTED;
}
