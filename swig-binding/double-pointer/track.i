%module track

%{
#include "track.h"
%}

%include "typemaps.i"

%typemap(in, numinputs=0) Track** (Track* temp){
  temp = NULL;
  $1 = &temp;
}

%typemap(argout) Track** {
  %append_output(SWIG_NewPointerObj(%as_voidptr(*$1), $*1_descriptor, SWIG_POINTER_OWN));
}

%ignore operator[];
%ignore operator=;

%include "track.h"