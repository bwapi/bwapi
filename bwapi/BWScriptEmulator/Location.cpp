#include "Location.h"

Location::Location(int left, int top, int right, int bottom)
  : topLeft(left,top)
  , bottomRight(right,bottom)
{};

Location::Location(BWAPI::Position leftTop, BWAPI::Position rightBottom)
  : topLeft(leftTop)
  , bottomRight(rightBottom)
{};

BWAPI::Position Location::center() const
{
  return (this->topLeft + this->bottomRight)/2;
}
int Location::left() const
{
  return this->topLeft.x;
}
int Location::top() const
{
  return this->topLeft.y;
}
int Location::right() const
{
  return this->bottomRight.x;
}
int Location::bottom() const
{
  return this->bottomRight.y;
}
