#ifndef VECTOR_HPP
#define VECTOR_HPP

typedef struct Foo {
  int x;
} Foo;
 
typedef struct Bar {
  int y;
  Foo f;           /* struct member */
} Bar;

#endif