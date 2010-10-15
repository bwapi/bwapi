#pragma once
#include "Shape.h"
#include <string>
namespace BWAPI
{
  class ShapeText : public Shape
  {
  public :
    ShapeText(int ctype, int x, int y, std::string text, char size);
    virtual void draw();
  private :
    char  size;
    int   ctype;
    int   x;
    int   y;
    std::string text;
  };
};
