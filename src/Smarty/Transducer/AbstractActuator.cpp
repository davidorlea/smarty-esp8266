#include "AbstractActuator.hpp"

std::vector<SmartyAbstractActuator*>* SmartyAbstractActuator::_list = nullptr;

const std::vector<SmartyAbstractActuator*>* SmartyAbstractActuator::getList() {
  if (_list == nullptr) {
    _list = new std::vector<SmartyAbstractActuator*>();
  }
  return _list;
}

SmartyAbstractActuator::SmartyAbstractActuator(const char* name)
: SmartyAbstractTransducer(name) {
  if (_list == nullptr) {
    _list = new std::vector<SmartyAbstractActuator*>();
  }
  _list->push_back(this);
  _listIndex = _list->size() - 1;
}

SmartyAbstractActuator::~SmartyAbstractActuator() {
  _list->erase(_list->begin() + _listIndex);
}

void SmartyAbstractActuator::addActivateCallback(SMARTY_ACTUATOR_CALLBACK_TYPE callback) {
  _activateCallbacks.push_back(callback);
}

void SmartyAbstractActuator::addDeactivateCallback(SMARTY_ACTUATOR_CALLBACK_TYPE callback) {
  _deactivateCallbacks.push_back(callback);
}
