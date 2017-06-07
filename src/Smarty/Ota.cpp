#include "Ota.hpp"

void SmartyOta::setup() {
  ArduinoOTA.onStart([]() {
    Serial << "Starting Smarty OTA Update ..." << endl;
  });
  ArduinoOTA.onEnd([]() {
    Serial << "... Finished Starting Smarty OTA Update" << endl;
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial << "Smarty OTA update error: ";
    switch (error) {
      case OTA_AUTH_ERROR:
        Serial << "Authentication Failed" << endl;
        break;
      case OTA_BEGIN_ERROR:
        Serial << "Begin Failed" << endl;
        break;
      case OTA_CONNECT_ERROR:
        Serial << "Connect Failed" << endl;
        break;
      case OTA_RECEIVE_ERROR:
        Serial << "Receive Failed" << endl;
        break;
      case OTA_END_ERROR:
        Serial << "End Failed" << endl;
        break;
    }
  });
  ArduinoOTA.begin();
}

void SmartyOta::loop() {
  ArduinoOTA.handle();
}
