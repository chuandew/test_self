#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstdint>
#include <vector>

class Vector {
public:
  Vector() = default;
  Vector(uint64_t, uint64_t);
  double abs();
  void display();

private:
  uint64_t x;
  uint64_t y;
};

class VectorUtil {
public:
  int add(std::vector<Vector>& vector, int i);

  int set(double& a);
};

#endif