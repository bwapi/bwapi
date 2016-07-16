#pragma once
#include <BWAPI.h>

class Location
{
public:
  Location(int left = 0, int top = 0, int right = 0, int bottom = 0);
  Location(BWAPI::Position leftTop, BWAPI::Position rightBottom);

  BWAPI::Position center() const;
  int left() const;
  int top() const;
  int right() const;
  int bottom() const;

  BWAPI::Position topLeft;
  BWAPI::Position bottomRight;
};
