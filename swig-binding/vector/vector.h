#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector {
public:
  Vector(int, int);
  double abs();
  void display();

private:
  int x;
  int y;
};

#endif