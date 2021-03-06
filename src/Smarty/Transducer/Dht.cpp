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

  if ((now - _lastReadTime) > DHT_READ_INTERVAL) {
    float temperature = _dht.getTemperature();
    float humidity = _dht.getHumidity();

    _readState.setTemperature(temperature, true);
    _readState.setHumidity(humidity, true);

    if (isnan(temperature) || isnan(humidity)) {
      _readState.setFailedReadingsCounter(_readState.getFailedReadingsCounter() + 1);
    } else {
      _readState.setSuccessfulReadingsCounter(_readState.getSuccessfulReadingsCounter() + 1);
    }

    _lastReadTime = now;
  }

  if ((now - _lastPublishTime > DHT_PUBLISH_INTERVAL) ) {
    _publishState.import(_readState);

    for (SMARTY_SENSOR_CALLBACK_TYPE callback: _stateCallbacks) {
      callback();
    }

    _readState.reset();
    _lastPublishTime = now;
  }

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
  _publishState.toJson(rootJson);
  return rootJson;
}
