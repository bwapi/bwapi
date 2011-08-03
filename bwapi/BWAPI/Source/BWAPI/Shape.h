#pragma once
#include <BWAPI/CoordinateType.h>
namespace BWAPI
{
  class Shape
  {
    public :
      virtual ~Shape() {};
      virtual void draw() = 0;
  };
};
