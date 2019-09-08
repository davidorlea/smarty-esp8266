#pragma once

#include <Arduino.h>
#include "AbstractState.hpp"

class SmartyDHTState : public SmartyAbstractState {
public:
  float getTemperature() const;
  float getHumidity() const;
  void setTemperature(float, bool = false);
  void setHumidity(float, bool = false);
  JsonObject& toJson(JsonObject&) override;
private:
  static float _filter(float, float);
  float _temperature{NAN};
  float _humidity{NAN};
};
