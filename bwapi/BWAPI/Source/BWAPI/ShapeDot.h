#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeDot : public Shape
  {
  public :
    ShapeDot(CoordinateType::Enum ctype, int x, int y, int color);
    virtual void draw();
  private :
    CoordinateType::Enum ctype;
    int x;
    int y;
    int color;
  };
}