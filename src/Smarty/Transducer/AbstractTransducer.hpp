#pragma once

#include <vector>
#include <Arduino.h>
#include <Streaming.h>

class SmartyAbstractTransducer {
public:
  const char* getName();
  virtual bool setup() = 0;
  virtual bool loop() = 0;
  virtual int state() = 0;
protected:
  const char* _name;
  SmartyAbstractTransducer(const char*);
  ~SmartyAbstractTransducer();
};
