#include <Arduino.h>
#include "Smarty/Smarty.hpp"

#define BUTTON1_PIN 5
#define RELAY1_PIN 14

Smarty smarty;
SmartyButton button1("button1", BUTTON1_PIN, SmartyButton::Mode::SWITCH);
SmartyBinaryActuator relay1("relay1", RELAY1_PIN, SmartyBinaryActuator::Wiring::INVERSE);

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

  button1.addStateCallback([]() {
    Serial << button1.getName() << F(" switched") << endl;
    relay1.toggle();
  });
  relay1.addActivateCallback([](bool changed) {
    if (changed) {
      Serial << relay1.getName() << F(" activated") << endl;
    }
  });
  relay1.addDeactivateCallback([](bool changed) {
    if (changed) {
      Serial << relay1.getName() << F(" deactivated") << endl;
    }
  });

  smarty.setup();

  Serial << F("... Finished main setup") << endl;
}

void loop() {
  smarty.loop();
}
