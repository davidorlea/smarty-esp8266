#include <Arduino.h>
#include "Smarty/Smarty.hpp"

// Configuration for custom board
// BUTTON1_PIN 12
// RELAY1_PIN 2

// Configuration for Sonoff board
Smarty smarty;
SmartyButton button1("button1", 0, SmartyButton::Mode::PUSH);
SmartyLed led1("led1", 13, SmartyLed::State::OFF);
SmartyRelay relay1("relay1", 12, SmartyRelay::State::OFF);
SmartyMqttPublication mqttButtonPub("smarty/demo/esp8266-1/button");
SmartyMqttPublication mqttPowerPub("smarty/demo/esp8266-1/power");
SmartyMqttSubscription mqttPowerSub("smarty/demo/esp8266-1/power/set");

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


  button1.setCallback([](uint8_t buttonState) {
    Serial << button1.getName() << F(" pushed") << endl;
    mqttButtonPub.setMessage("2");
    mqttButtonPub.ready();
    relay1.toggle();
  });
  led1.setActivateCallback([](bool change) {
    if (change) {
      Serial << led1.getName() << F(" activated") << endl;
    }
  });
  led1.setDeactivateCallback([](bool change) {
    if (change) {
      Serial << led1.getName() << F(" deactivated") << endl;
    }
  });
  relay1.setActivateCallback([](bool change) {
    if (change) {
      Serial << relay1.getName() << F(" activated") << endl;
    }
    mqttPowerPub.setMessage("1");
    mqttPowerPub.ready();
    led1.activate();
  });
  relay1.setDeactivateCallback([](bool change) {
    if (change) {
      Serial << relay1.getName() << F(" deactivated") << endl;
    }
    mqttPowerPub.setMessage("0");
    mqttPowerPub.ready();
    led1.deactivate();
  });
  mqttPowerSub.setCallback([](const char *topic, const char *message) {
    if (strcmp(message, "0") == 0) {
      relay1.deactivate();
    } else if (strcmp(message, "1") == 0) {
      relay1.activate();
    } else if (strcmp(message, "2") == 0) {
      relay1.toggle();
    }
  });

  smarty.setup();

  Serial << F("... Finished main setup") << endl;
}

void loop() {
  smarty.loop();
}
