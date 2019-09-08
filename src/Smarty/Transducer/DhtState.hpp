#pragma once

#include <Arduino.h>
#include "AbstractState.hpp"

class SmartyDHTState : public SmartyAbstractState {
public:
  float getTemperature() const;
  float getHumidity() const;
  int getFailedReadingsCounter() const;
  int getSuccessfulReadingsCounter() const;
  void setTemperature(float, bool = false);
  void setHumidity(float, bool = false);
  void setFailedReadingsCounter(int);
  void setSuccessfulReadingsCounter(int);
  JsonObject& toJson(JsonObject&) override;
private:
  static float _filter(float, float);
  float _temperature{NAN};
  float _humidity{NAN};
  int _failedReadingsCounter = 0;
  int _successfulReadingsCounter = 0;
};
