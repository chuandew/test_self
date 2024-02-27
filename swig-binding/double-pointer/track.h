#pragma once

class Track {
public:
  Track() = default;
  Track(int, int);
  double abs();
  void display();

private:
  int x;
  int y;
};

class TrackUtil {
public:
  static int  new_track(Track** v);
};