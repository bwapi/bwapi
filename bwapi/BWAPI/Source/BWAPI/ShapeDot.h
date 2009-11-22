#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeDot : public Shape
  {
  public :
    ShapeDot(int ctype, int x, int y, int color);
    virtual void draw();
  private :
    int ctype;
    int x;
    int y;
    int color;
  };
}