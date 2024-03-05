%module example 

%{
#include "example.h"
%}

%include <stdint.i>
%include <std_string.i>
%include <std_vector.i>
%include <typemaps.i>

%template(PersonVector)  std::vector<Person>;

// Custom typemap to convert a Python list of dictionaries to const std::vector<Person>&
%typemap(in) std::vector<Person> {
    if (!PyList_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list.");
        SWIG_fail;
    }

    int size = PyList_Size($input);
    $1.resize(size);

    for (int i = 0; i < size; i++) {
        PyObject* item = PyList_GetItem($input, i);

        if (!PyDict_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "Expected a dictionary.");
            SWIG_fail;
        }

        PyObject* nameObj = PyDict_GetItemString(item, "name");
        PyObject* ageObj = PyDict_GetItemString(item, "age");

        if (!nameObj || !ageObj || !PyUnicode_Check(nameObj) || !PyLong_Check(ageObj)) {
            PyErr_SetString(PyExc_TypeError, "Invalid dictionary format.");
            SWIG_fail;
        }

        std::string name = PyUnicode_AsUTF8(nameObj);
        int age = PyLong_AsLong(ageObj);

        $1[i].name = name;
        $1[i].age = age;
    }
}

%typemap(in) const std::vector<Person>& (std::vector<Person> tempVector){
    if (!PyList_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list.");
        SWIG_fail;
    }

    int size = PyList_Size($input);
    tempVector.reserve(size);

    for (int i = 0; i < size; i++) {
        PyObject* item = PyList_GetItem($input, i);

        if (!PyDict_Check(item)) {
            PyErr_SetString(PyExc_TypeError, "Expected a dictionary.");
            SWIG_fail;
        }

        PyObject* nameObj = PyDict_GetItemString(item, "name");
        PyObject* ageObj = PyDict_GetItemString(item, "age");

        if (!nameObj || !ageObj || !PyUnicode_Check(nameObj) || !PyLong_Check(ageObj)) {
            PyErr_SetString(PyExc_TypeError, "Invalid dictionary format.");
            SWIG_fail;
        }

        std::string name = PyUnicode_AsUTF8(nameObj);
        int age = PyLong_AsLong(ageObj);

        Person person;
        person.name = name;
        person.age = age;

        tempVector.push_back(person);
    }

    $1 = &tempVector;
}

%typemap(freearg) const std::vector<Person>& {
    // if ($1) delete $1;
}

%include "example.h"