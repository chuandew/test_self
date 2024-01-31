#include "mylib.h"

namespace test {
namespace pybind {
Pet::Pet(const std::string &name) : name(name) {}

void Pet::setName(const std::string &name_) { name = name_; }

const std::string &Pet::getName() const { return name; }
} // namespace pybind
} // namespace test