#pragma once
#include "Shape.h"
#include <string>
namespace BWAPI
{
  class ShapeText : public Shape
  {
  public :
    ShapeText(int ctype, int x, int y, std::string text);
    virtual void draw();
  private :
    int ctype;
    int x;
    int y;
    std::string text;
  };
};
