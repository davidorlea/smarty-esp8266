#include "AbstractSensor.hpp"

std::vector<SmartyAbstractSensor*>* SmartyAbstractSensor::_list = nullptr;

const std::vector<SmartyAbstractSensor*>* SmartyAbstractSensor::getList() {
  if (_list == nullptr) {
    _list = new std::vector<SmartyAbstractSensor*>();
  }
  return _list;
}

SmartyAbstractSensor::SmartyAbstractSensor(const char* name)
: SmartyAbstractTransducer(name) {
  if (_list == nullptr) {
    _list = new std::vector<SmartyAbstractSensor*>();
  }
  _list->push_back(this);
  _listIndex = _list->size() - 1;
}

SmartyAbstractSensor::~SmartyAbstractSensor() {
  _list->erase(_list->begin() + _listIndex);
}

void SmartyAbstractSensor::addStateCallback(SMARTY_SENSOR_CALLBACK_TYPE callback) {
  _stateCallbacks.push_back(callback);
}
