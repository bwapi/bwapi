#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeCircle : public Shape
  {
    public :
      ShapeCircle(int ctype, int x, int y, int radius, int color, bool isSolid = false);
      virtual void draw();
    private :
      int ctype;
      int x;
      int y;
      int radius;
      int color;
      bool isSolid;
  };
};
