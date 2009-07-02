#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeBox : public Shape
  {
  public :
    ShapeBox(int coordinateType, int left, int top, int right, int bottom, int color, bool isSolid=false);
    virtual void draw();
  private :
    int coordinateType;
    int left;
    int top;
    int right;
    int bottom;
    int color;
    bool isSolid;
  };
}