#include "AbstractTransducer.hpp"

SmartyAbstractTransducer::SmartyAbstractTransducer(const char* name)
: _name(name) {
};

SmartyAbstractTransducer::~SmartyAbstractTransducer() {
  Serial << "ERROR: Destruction of Smarty Transducer not allowed!" << endl;
  abort();
}

const char* SmartyAbstractTransducer::getName() {
  return _name;
}
