#include "Dht.hpp"

SmartyDHT::SmartyDHT(const char* name, const uint8_t port, const Type type)
: SmartyAbstractSensor(name)
, _port(port)
, _type(type) {
}

bool SmartyDHT::setup() {
  _dht.setup(_port, (DHT::DHT_MODEL_t) _type);
  return true;
}

bool SmartyDHT::loop() {
  unsigned long now = millis();
  if ((now - _lastReadTime) <= DHT_READ_DELAY) {
    return false;
  }

  float temperature = _dht.getTemperature();
  float humidity = _dht.getHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    _state.clearHumidity();
    _state.clearTemperature();
    return false;
  }

  _state.setTemperature(temperature);
  _state.setHumidity(humidity);

  for (SMARTY_SENSOR_CALLBACK_TYPE callback: _stateCallbacks) {
    callback();
  }

  _lastReadTime = now;
  return true;
}

JsonObject& SmartyDHT::toJson(JsonBuffer& jsonBuffer) {
  JsonObject& rootJson = SmartyAbstractSensor::toJson(jsonBuffer);
  switch(_type) {
    case Type::DHT11:
      rootJson["type"] = "dht11";
      break;
    case Type::DHT22:
      rootJson["type"] = "dht22";
      break;
  }
  _state.toJson(rootJson);
  return rootJson;
}
