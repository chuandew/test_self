

#include "header.h"

#include <stdio.h>

void C() {
  B();
  printf("I'm C func");
}

int main() {
  printf("I'm C main\n");
  C();
}