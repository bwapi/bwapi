#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeLine : public Shape
  {
  public :
    ShapeLine(int coordinateType, int x1, int y1, int x2, int y2, int color);
    virtual void draw();
  private :
    int coordinateType;
    int x1;
    int y1;
    int x2;
    int y2;
    int color;
  };
}