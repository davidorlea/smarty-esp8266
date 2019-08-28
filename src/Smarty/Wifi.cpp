#include "Wifi.hpp"

SmartyWifi::SmartyWifi() {
  auto* composedHostname = (char*) malloc(21 + 1);
  sprintf(composedHostname, "smarty-esp8266-%06x", ESP.getChipId());
  _hostname = composedHostname;
}

void SmartyWifi::setSSID(const char* ssid) {
  _ssid = ssid;
}

void SmartyWifi::setPassword(const char* password) {
  _password = password;
}

void SmartyWifi::setHostname(const char* hostname) {
  free((void*) _hostname);
  _hostname = hostname;
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

String SmartyWifi::getHostName() {
  return WiFi.hostname();
}

void SmartyWifi::setup() {
  _clearDefaultSettings();
  _clearCurrentSettings();
}

bool SmartyWifi::loop() {
  return _connect();
}

JsonObject& SmartyWifi::toJson(JsonObject& rootJson) {
  JsonObject& wifiJson = rootJson.createNestedObject("wifi");
  wifiJson["ssid"] = getSSID();
  wifiJson["rssi"] = getRSSI();
  wifiJson["ip"] = getIpAddress();
  wifiJson["hostname"] = getHostName();
  return wifiJson;
}

bool SmartyWifi::_connect() {
  if (!_ssid || !_password) {
    return false;
  }
  if (!_isConnected()) {
    if (!_isWaitingForConnection) {
      Serial << "(Re-)Connecting to Wifi ..." << endl;
      _isWaitingForConnection = true;
      WiFi.persistent(false);
      WiFi.begin(_ssid, _password);
      WiFi.hostname(_hostname);
    }
    return false;
  } else {
    if (_isWaitingForConnection) {
      Serial << "... successfully (re-)connected to Wifi" << endl;
      _isWaitingForConnection = false;
      _printNetworkData();
    }
    return true;
  }
}

void SmartyWifi::_clearDefaultSettings() {
  // Access default settings (saved in flash)
  WiFi.persistent(true);
  // Set default operating mode to none (OFF)
  WiFi.mode(WIFI_OFF);
  // Clear default station (STA) settings (and do not touch default Wifi Mode)
  WiFi.disconnect(false);
  // Clear default access point (AP) settings (and do not touch default Wifi Mode)
  WiFi.softAPdisconnect(false);
}

void SmartyWifi::_clearCurrentSettings() {
  // Access current settings (not saved in flash)
  WiFi.persistent(false);
  // Set current operating mode to none (OFF)
  WiFi.mode(WIFI_OFF);
  // Clear current station (STA) settings (and do not touch current Wifi Mode)
  WiFi.disconnect(false);
  // Clear current access point (AP) settings (and do not current touch Wifi Mode)
  WiFi.softAPdisconnect(false);
}

void SmartyWifi::_printCurrentAndDefaultSettings() {
  struct station_config current_conf{};
  struct station_config default_conf{};
  wifi_station_get_config(&current_conf);
  wifi_station_get_config_default(&default_conf);

  const char* wifiModes[] = { "OFF", "STA", "AP", "STA+AP" };

  Serial << "Wifi current operating mode: " << wifiModes[wifi_get_opmode()] << endl;
  Serial << "Wifi default operating mode (saved in flash): " << wifiModes[wifi_get_opmode_default()] << endl;
  Serial << "Wifi Station current SSID: " << *current_conf.ssid << endl;
  Serial << "Wifi Station current password: " << *current_conf.password << endl;
  Serial << "Wifi Station default SSID (saved in flash): " << *default_conf.ssid << endl;
  Serial << "Wifi Station default password (saved in flash): " << *default_conf.password << endl;
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
