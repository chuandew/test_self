#include "vector.h"
#include <cmath>
#include <cstdint>
#include <iostream>
using namespace std;

Vector::Vector(uint64_t a, uint64_t b) {
  x = a;
  y = b;
}

void Vector::display() const { cout << "(" << x << ',' << y << ')' << endl; }

double Vector::abs() { return sqrt(x * x + y * y); }

int VectorUtil::add(std::vector<Vector> &vector, int i) {
  for (int j = 0; j < i; j++) {
    vector.push_back(Vector(j, j));
  }
  return i;
}

int VectorUtil::set(double &a) {
  a = 42.;
  return 1;
}

void VectorUtil::display(const std::vector<Vector> &vector) {
  for(auto &v : vector) {
    v.display();
  }
}