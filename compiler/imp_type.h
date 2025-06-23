#ifndef IMP_TYPE
#define IMP_TYPE

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <list>
#include <vector>

using namespace std;

class ImpType {
public:
    string TType;
    vector<string> types;
    ImpType(string s);
    ImpType();
    bool match(const ImpType&);
    bool set_basic_type(string s);
    bool set_fun_type(list<string> slist, string s);
};

#endif