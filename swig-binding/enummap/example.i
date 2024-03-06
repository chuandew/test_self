%module example 

%{
#include "example.h"
%}

%include <stdint.i>
%include <std_map.i>

%include <typemaps.i>

%template(MyEnumMap)  std::map<MyEnum, int>;

%include "example.h"