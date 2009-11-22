#pragma once
#include "Shape.h"
namespace BWAPI
{
  class ShapeTriangle : public Shape
  {
    public :
      ShapeTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, int color, bool isSolid = false);
      virtual void draw();

    private :
      virtual void swap(int& a0, int& a1);
      virtual int rnd(float x);
      int ctype;
      int ax, ay;
      int bx, by;
      int cx, cy;
      int color;
      bool isSolid;
  };
}