#pragma once
#include "Shape.h"
#include <string>
namespace BWAPI
{
  class ShapeText : public Shape
  {
  public :
    ShapeText(CoordinateType::Enum ctype, int x, int y, std::string text);
    virtual void draw();
  private :
    CoordinateType::Enum ctype;
    int x;
    int y;
    std::string text;
  };
}