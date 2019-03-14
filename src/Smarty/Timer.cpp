#include "Timer.hpp"

std::vector<SmartyTimer*>* SmartyTimer::_list = nullptr;

std::vector<SmartyTimer*>* SmartyTimer::getList() {
  if (_list == nullptr) {
    _list = new std::vector<SmartyTimer*>();
  }
  return _list;
}

SmartyTimer::SmartyTimer(uint32_t interval)
: _interval(interval) {
  if (_list == nullptr) {
    _list = new std::vector<SmartyTimer*>();
  }
  _list->push_back(this);
  _listIndex = _list->size() - 1;
}

SmartyTimer::~SmartyTimer() {
  _list->erase(_list->begin() + _listIndex);
}

void SmartyTimer::setCallback(SMARTY_TIMER_CALLBACK_TYPE callback) {
  _callback = callback;
}

void SmartyTimer::setCondition(SMARTY_TIMER_CONDITION_TYPE condition) {
  _condition = condition;
}

void SmartyTimer::update() {
  if (_condition && !_condition()) {
    return;
  }
  uint32_t now = millis();
  if (_lastUpdate == 0 || now - _lastUpdate >= _interval) {
    _lastUpdate = now;
    if (_callback) {
      _callback();
    }
  }
}
