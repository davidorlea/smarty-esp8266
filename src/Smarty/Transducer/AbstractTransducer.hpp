#pragma once

#include <cstdint>
#include <vector>

class SmartyAbstractTransducer {
public:
  const char* getName();
  virtual bool setup() = 0;
  virtual bool loop() = 0;
  virtual uint8_t state() = 0;
protected:
  const char* _name;
  SmartyAbstractTransducer(const char*);
};
