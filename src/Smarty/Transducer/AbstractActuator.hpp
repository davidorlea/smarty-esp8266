#pragma once

#include <vector>
#include "AbstractTransducer.hpp"

class SmartyAbstractActuator : public SmartyAbstractTransducer {
public:
  static const std::vector<SmartyAbstractActuator*>* getList();
  virtual bool activate() = 0;
  virtual bool deactivate() = 0;
  virtual bool toggle() = 0;
  virtual bool parseState(int) = 0;
protected:
  static std::vector<SmartyAbstractActuator*>* _list;
  SmartyAbstractActuator(const char*);
};
