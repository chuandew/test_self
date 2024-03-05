%module example

%{
#include "example.h"
%}

%include <stdint.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>
%include <std_unique_ptr.i>
%include <std_shared_ptr.i>
%include <typemaps.i>

%template(PersonVector)  std::vector<Person>;

// Custom typemap to convert a Python list of tuples to std::vector<Person>
%typemap(in) std::vector<Person> {
    if (!PyList_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list.");
        SWIG_fail;
    }

    int size = PyList_Size($input);
    $1.resize(size);

    for (int i = 0; i < size; i++) {
        PyObject* item = PyList_GetItem($input, i);

        if (!PyTuple_Check(item) || PyTuple_Size(item) != 2) {
            PyErr_SetString(PyExc_TypeError, "Expected a tuple of size 2.");
            SWIG_fail;
        }

        PyObject* nameObj = PyTuple_GetItem(item, 0);
        PyObject* ageObj = PyTuple_GetItem(item, 1);

        if (!PyUnicode_Check(nameObj) || !PyLong_Check(ageObj)) {
            PyErr_SetString(PyExc_TypeError, "Invalid tuple format.");
            SWIG_fail;
        }

        std::string name = PyUnicode_AsUTF8(nameObj);
        int age = PyLong_AsLong(ageObj);

        $1[i].name = name;
        $1[i].age= age;
    }
}

%include "example.h"