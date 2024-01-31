#include <cstdint>
// #include <pybind11/embed.h>
#include "mylib.h"
#include <pybind11/pybind11.h>

using namespace test::pybind;

namespace py = pybind11;

// int multiply(uint32_t i, int j) { return i * j; }

// PYBIND11_MODULE(example, m) {
//   m.doc() = "pybind11 example plugin"; // optional module docstring
//   m.def("multiply", &multiply, "A function which multiplies two numbers");
// }

PYBIND11_MODULE(example, m) {
  py::class_<Pet>(m, "Pet")
      .def(py::init<const std::string &>())
      .def("setName", &Pet::setName)
      .def("getName", &Pet::getName)
      .def("__repr__",
           [](const Pet &a) { return "<example.Pet named '" + a.name + "'>"; });
}