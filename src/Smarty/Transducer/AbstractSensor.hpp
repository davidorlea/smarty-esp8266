#pragma once

#include <vector>
#include "AbstractTransducer.hpp"

class SmartyAbstractSensor : public SmartyAbstractTransducer {
public:
  static const std::vector<SmartyAbstractSensor*>* getList();
protected:
  static std::vector<SmartyAbstractSensor*>* _list;
  SmartyAbstractSensor(const char*);
  virtual ~SmartyAbstractSensor();
private:
  std::vector<SmartyAbstractSensor*>::size_type _listIndex;
};
