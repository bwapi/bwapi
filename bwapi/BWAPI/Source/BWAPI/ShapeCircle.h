#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeCircle : public Shape
  {
    public :
      ShapeCircle(CoordinateType::Enum ctype, int x, int y, int radius, int color, bool isSolid = false);
      virtual void draw();
    private :
      CoordinateType::Enum ctype;
      int x;
      int y;
      int radius;
      int color;
      bool isSolid;
  };
}