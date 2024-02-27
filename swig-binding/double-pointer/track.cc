
#include "track.h"
#include <cmath>
#include <iostream>

Track::Track(int a, int b) {
  x = a;
  y = b;
}

void Track::display() {
  std::cout << "(" << x << ',' << y << ')' << std::endl;
}

double Track::abs() { return std::sqrt(x * x + y * y); }

int TrackUtil::new_track(Track **v) {
  Track *t = new Track(1, 2);
  *v = t;
  return 0;
}