%module cal

%{
#include "cal.h"
%}

%include <std_vector.i>
%template(FloatVector) std::vector<float>;

%naturalvar myStruct::arr1;
// %naturalvar myStruct::arr2;

%include "cal.h"
