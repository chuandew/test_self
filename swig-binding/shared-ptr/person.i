%module person

%{
#include "person.h"
%}

%include "std_shared_ptr.i"
%include "std_string.i"

%shared_ptr(Person)

%include "person.h"
