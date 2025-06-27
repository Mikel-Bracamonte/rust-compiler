#include "imp_type.h"


ImpType::ImpType(string s) {
  ttype = s;
}

ImpType::ImpType() {}

bool ImpType::match(const ImpType& t) {
  if (this->ttype != t.ttype) return false;
  return true;
}

bool ImpType::set_basic_type(string s) {
  bool ret=true;
  return ret;
}


bool ImpType::set_fun_type(list<string> slist, string s) {
  bool ret=true;
  return ret;
}