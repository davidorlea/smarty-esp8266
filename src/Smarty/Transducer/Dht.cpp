#include "Dht.hpp"

SmartyDHT::SmartyDHT(const char* name, const uint8_t port, const Type type)
: SmartyAbstractSensor(name)
, _dht(port, (uint8_t) type)
, _port(port)
, _type(type) {
}

bool SmartyDHT::setup() {
  _dht.begin();
  return true;
}

bool SmartyDHT::loop() {
  unsigned long now = millis();
  if ((now - _lastReadTime) <= DHT_READ_DELAY) {
    return false;
  }

  float temperature = _dht.readTemperature(false);
  float humidity = _dht.readHumidity();
  float heatIndex = _dht.computeHeatIndex(temperature, humidity, false);

  if (isnan(temperature) || isnan(humidity) || isnan(heatIndex)) {
    _state.clearHumidity();
    _state.clearTemperature();
    _state.clearHeatIndex();
    return false;
  }

  _state.setTemperature(temperature);
  _state.setHumidity(humidity);
  _state.setHeatIndex(heatIndex);

  for (SMARTY_SENSOR_CALLBACK_TYPE callback: _stateCallbacks) {
    callback();
  }

  _lastReadTime = now;
  return true;
}

JsonObject& SmartyDHT::toJson(JsonBuffer& jsonBuffer) {
  JsonObject& rootJson = SmartyAbstractSensor::toJson(jsonBuffer);
  switch(_type) {
    case Type::DHT_11:
      rootJson["type"] = "dht11";
      break;
    case Type::DHT_12:
      rootJson["type"] = "dht12";
      break;
    case Type::DHT_21:
      rootJson["type"] = "dht21";
      break;
    case Type::DHT_22:
      rootJson["type"] = "dht22";
      break;
  }
  _state.toJson(rootJson);
  return rootJson;
}
