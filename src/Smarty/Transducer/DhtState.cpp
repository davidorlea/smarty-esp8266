#include "DhtState.hpp"

float SmartyDHTState::getTemperature() const {
  return _temperature;
}

float SmartyDHTState::getHumidity() const {
  return _humidity;
}

int SmartyDHTState::getFailedReadingsCounter() const {
  return _failedReadingsCounter;
}

int SmartyDHTState::getSuccessfulReadingsCounter() const {
  return _successfulReadingsCounter;
}

void SmartyDHTState::setTemperature(float temperature, bool filter) {
  _temperature = filter ? _filter(_temperature, temperature) : temperature;
}

void SmartyDHTState::setHumidity(float humidity, bool filter) {
  _humidity = filter ? _filter(_humidity, humidity) : humidity;
}

void SmartyDHTState::setFailedReadingsCounter(int failedReadingsCounter) {
  _failedReadingsCounter = failedReadingsCounter;
}

void SmartyDHTState::setSuccessfulReadingsCounter(int successfulReadingsCounter) {
  _successfulReadingsCounter = successfulReadingsCounter;
}

void SmartyDHTState::import(SmartyDHTState& state) {
  _temperature = state.getTemperature();
  _humidity = state.getHumidity();
  _failedReadingsCounter = state.getFailedReadingsCounter();
  _successfulReadingsCounter = state.getSuccessfulReadingsCounter();
}

void SmartyDHTState::reset() {
  _temperature = NAN;
  _humidity = NAN;
  _failedReadingsCounter = 0;
  _successfulReadingsCounter = 0;
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
  stateJson["failedReadings"] = _failedReadingsCounter;
  stateJson["successfulReadings"] = _successfulReadingsCounter;
  return stateJson;
}

float SmartyDHTState::_filter(float oldValue, float newValue) {
  if (isnan(oldValue) || isnan(newValue)) {
    return newValue;
  }
  return (newValue + oldValue * 3) / 4;
}
