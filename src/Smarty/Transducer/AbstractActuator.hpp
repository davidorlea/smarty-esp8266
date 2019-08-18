#pragma once

#include <functional>
#include <vector>
#include <ArduinoJson.h>
#include "AbstractTransducer.hpp"

#define SMARTY_ACTUATOR_CALLBACK_TYPE std::function<void (bool changed)>

class SmartyAbstractActuator : public SmartyAbstractTransducer {
public:
  static const std::vector<SmartyAbstractActuator*>* getList();
  void addActivateCallback(SMARTY_ACTUATOR_CALLBACK_TYPE);
  void addDeactivateCallback(SMARTY_ACTUATOR_CALLBACK_TYPE);
  JsonObject& toJson(JsonBuffer&) override;
  virtual bool activate() = 0;
  virtual bool deactivate() = 0;
  virtual bool toggle() = 0;
  virtual bool parseState(int) = 0;
protected:
  static std::vector<SmartyAbstractActuator*>* _list;
  explicit SmartyAbstractActuator(const char*);
  virtual ~SmartyAbstractActuator();
  std::vector<SMARTY_ACTUATOR_CALLBACK_TYPE> _activateCallbacks;
  std::vector<SMARTY_ACTUATOR_CALLBACK_TYPE> _deactivateCallbacks;
private:
  std::vector<SmartyAbstractActuator*>::size_type _listIndex;
};
