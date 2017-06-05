#include "AbstractTransducer.hpp"

SmartyAbstractTransducer::SmartyAbstractTransducer(const char* name)
: _name(name) {
};

const char* SmartyAbstractTransducer::getName() {
  return _name;
}
