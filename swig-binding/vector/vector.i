%module vector
%{
#include "vector.h"
%}

%include <std_vector.i>
%include <typemaps.i>
%include <stdint.i>

%template(CustomeVector)  std::vector<Vector>;

%apply double& OUTPUT  { double& a };

%include "vector.h"