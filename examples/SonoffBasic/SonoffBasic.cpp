#include <Arduino.h>
#include "Smarty/Smarty.hpp"

#define BUTTON1_PIN 0
#define LED1_PIN 13
#define RELAY1_PIN 12

Smarty smarty;
SmartyButton button1("button1", BUTTON1_PIN, SmartyButton::Mode::PUSH);
SmartyLed led1("led1", LED1_PIN, SmartyLed::State::OFF);
SmartyRelay relay1("relay1", RELAY1_PIN, SmartyRelay::State::OFF);

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

  button1.addStateCallback([](uint8_t state) {
    Serial << button1.getName() << F(" pushed") << endl;
    relay1.toggle();
  });
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
  relay1.addActivateCallback([](bool changed) {
    if (changed) {
      Serial << relay1.getName() << F(" activated") << endl;
    }
    led1.activate();
  });
  relay1.addDeactivateCallback([](bool changed) {
    if (changed) {
      Serial << relay1.getName() << F(" deactivated") << endl;
    }
    led1.deactivate();
  });

  smarty.setup();

  Serial << F("... Finished main setup") << endl;
}

void loop() {
  smarty.loop();
}