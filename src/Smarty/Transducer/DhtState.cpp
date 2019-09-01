#include "DhtState.hpp"

void SmartyDHTState::setTemperature(float temperature, bool filter) {
  _temperature = filter ? _filter(_temperature, temperature) : temperature;
}

void SmartyDHTState::setHumidity(float humidity, bool filter) {
  _humidity = filter ? _filter(_humidity, humidity) : humidity;
}

void SmartyDHTState::clearTemperature() {
  _temperature = NAN;
}

void SmartyDHTState::clearHumidity() {
  _humidity = NAN;
}

JsonObject& SmartyDHTState::toJson(JsonObject& rootJson) {
  JsonObject& stateJson = rootJson.createNestedObject("state");
  if (!isnan(_temperature)) {
    stateJson["temperature"] = _temperature;
  } else {
    stateJson["temperature"] = RawJson("null");
  }
  if (!isnan(_humidity)) {
    stateJson["humidity"] = _humidity;
  } else {
    stateJson["humidity"] = RawJson("null");
  }
  return stateJson;
}

float SmartyDHTState::_filter(float oldValue, float newValue) {
  if (isnan(oldValue)) {
    return newValue;
  }
  return (newValue + oldValue * 3) / 4;
}

