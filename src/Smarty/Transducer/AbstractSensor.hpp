#pragma once

#include <functional>
#include <vector>
#include <ArduinoJson.h>
#include "AbstractTransducer.hpp"

#define SMARTY_SENSOR_CALLBACK_TYPE std::function<void (void)>

class SmartyAbstractSensor : public SmartyAbstractTransducer {
public:
  static const std::vector<SmartyAbstractSensor*>* getList();
  void addStateCallback(SMARTY_SENSOR_CALLBACK_TYPE);
  JsonObject& toJson(JsonBuffer&) override;
protected:
  static std::vector<SmartyAbstractSensor*>* _list;
  explicit SmartyAbstractSensor(const char*);
  virtual ~SmartyAbstractSensor();
  std::vector<SMARTY_SENSOR_CALLBACK_TYPE> _stateCallbacks;
private:
  std::vector<SmartyAbstractSensor*>::size_type _listIndex;
};
