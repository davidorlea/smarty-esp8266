#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <Arduino.h>

#define SMARTY_TIMER_CALLBACK_TYPE std::function<void (void)>
#define SMARTY_TIMER_CONDITION_TYPE std::function<bool (void)>

class SmartyTimer {
public:
  static std::vector<SmartyTimer*>* getList();
  explicit SmartyTimer(uint32_t);
  ~SmartyTimer();
  void setCallback(SMARTY_TIMER_CALLBACK_TYPE);
  void setCondition(SMARTY_TIMER_CONDITION_TYPE);
  void update();
private:
  static std::vector<SmartyTimer*>* _list;
  std::vector<SmartyTimer*>::size_type _listIndex;
  uint32_t _interval;
  uint32_t _lastUpdate;
  SMARTY_TIMER_CALLBACK_TYPE _callback = nullptr;
  SMARTY_TIMER_CONDITION_TYPE _condition = nullptr;
};
