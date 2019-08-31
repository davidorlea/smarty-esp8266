#include "DhtState.hpp"

void SmartyDHTState::setTemperature(float temperature, bool filter) {
  _temperature = filter ? _filter(_temperature, temperature) : temperature;
}

void SmartyDHTState::setHumidity(float humidity, bool filter) {
  _humidity = filter ? _filter(_humidity, humidity) : humidity;
}

void SmartyDHTState::setHeatIndex(float heatIndex, bool filter) {
  _heatIndex = filter ? _filter(_heatIndex, heatIndex) : heatIndex;
}

void SmartyDHTState::clearTemperature() {
  _temperature = NAN;
}

void SmartyDHTState::clearHumidity() {
  _humidity = NAN;
}

void SmartyDHTState::clearHeatIndex() {
  _heatIndex = NAN;
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
  if (!isnan(_heatIndex)) {
    stateJson["heatIndex"] = _heatIndex;
  } else {
    stateJson["heatIndex"] = RawJson("null");
  }
  return stateJson;
}

float SmartyDHTState::_filter(float oldValue, float newValue) {
  if (isnan(oldValue)) {
    return newValue;
  }
  return (newValue + oldValue * 3) / 4;
}

