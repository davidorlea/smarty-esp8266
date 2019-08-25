#include <Arduino.h>
#include "Smarty/Smarty.hpp"

#if defined DEVICE_CUSTOM_V2 || defined DEVICE_CUSTOM_V3
#define BUTTON1_PIN 14
#define LED1_PIN 2
#define RELAY1_PIN 16
#endif

#if defined DEVICE_SONOFF_BASIC || defined DEVICE_SONOFF_S20
#define BUTTON1_PIN 0
#define LED1_PIN 13
#define RELAY1_PIN 12
#endif

Smarty smarty;

#if defined DEVICE_CUSTOM_V2 || defined DEVICE_CUSTOM_V3
SmartyButton button1("button1", BUTTON1_PIN, SmartyButton::Mode::SWITCH);
#endif

#if defined DEVICE_SONOFF_BASIC || defined DEVICE_SONOFF_S20
SmartyButton button1("button1", BUTTON1_PIN, SmartyButton::Mode::PUSH);
#endif

#if defined DEVICE_CUSTOM_V2 || defined DEVICE_SONOFF_BASIC || defined DEVICE_SONOFF_S20
SmartyBinaryActuator relay1("relay1", RELAY1_PIN, SmartyBinaryActuator::Wiring::REGULAR);
#endif

#if defined DEVICE_CUSTOM_V3
SmartyBinaryActuator relay1("relay1", RELAY1_PIN, SmartyBinaryActuator::Wiring::INVERSE);
#endif

#if defined DEVICE_SONOFF_BASIC || defined DEVICE_SONOFF_S20
SmartyBinaryActuator led1("led1", LED1_PIN, SmartyBinaryActuator::Wiring::INVERSE);
#endif

void waitForSerialMonitoring() {
  for (int i = 0; i < 5; i++) {
    Serial << ".";
    delay(1000);
  }
  Serial << endl;
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  waitForSerialMonitoring();

  Serial << F("Starting main setup ...") << endl;

  smarty.setFirmwareName("Smarty Home Automation");
  smarty.setFirmwareVersion("0.1.0");

  #if defined DEVICE_CUSTOM_V2 || defined DEVICE_CUSTOM_V3 || defined DEVICE_SONOFF_BASIC || defined DEVICE_SONOFF_S20
  button1.addStateCallback([](uint8_t state) {
    Serial << button1.getName() << F(" pushed") << endl;
    relay1.toggle();
  });
  #endif

  #if defined DEVICE_SONOFF_BASIC || defined DEVICE_SONOFF_S20
  led1.addActivateCallback([](bool changed) {
    if (changed) {
      Serial << led1.getName() << F(" activated") << endl;
    }
  });
  led1.addDeactivateCallback([](bool changed) {
    if (changed) {
      Serial << led1.getName() << F(" deactivated") << endl;
    }
  });
  #endif

  #if defined DEVICE_CUSTOM_V2 || defined DEVICE_CUSTOM_V3 || defined DEVICE_SONOFF_BASIC || defined DEVICE_SONOFF_S20
  relay1.addActivateCallback([](bool changed) {
    if (changed) {
      Serial << relay1.getName() << F(" activated") << endl;
    }
    #if defined DEVICE_SONOFF_BASIC
    led1.activate();
    #endif
  });
  relay1.addDeactivateCallback([](bool changed) {
    if (changed) {
      Serial << relay1.getName() << F(" deactivated") << endl;
    }
    #if defined DEVICE_SONOFF_BASIC
    led1.deactivate();
    #endif
  });
  #endif

  smarty.setup();

  Serial << F("... Finished main setup") << endl;
}

void loop() {
  smarty.loop();
}
