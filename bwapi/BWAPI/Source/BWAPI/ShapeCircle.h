#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeCircle : public Shape
  {
  public :
    ShapeCircle(int coordinateType, int x, int y, int radius, int color, bool isSolid=false);
    virtual void draw();
  private :
    int coordinateType;
    int x;
    int y;
    int radius;
    int color;
    bool isSolid;
  };
}