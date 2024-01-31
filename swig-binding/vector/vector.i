%module vector
%{
#include "vector.h"
%}

class Vector{
public:
    Vector(int,int);
    double abs();
    void display();
private:
    int x;
    int y;
};