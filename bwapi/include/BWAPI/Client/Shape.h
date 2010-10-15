#pragma once
#include "ShapeType.h"

namespace BWAPIC
{
  struct Shape
  {
    Shape(ShapeType::Enum _shapeType, int _ctype, int _x1, int _y1, int _x2, int _y2, int _extra1, int _extra2, int _color, bool _isSolid)
    {
      type=_shapeType;
      ctype=_ctype;
      x1=_x1;
      y1=_y1;
      x2=_x2;
      y2=_y2;
      extra1=_extra1;
      extra2=_extra2;
      color=_color;
      isSolid=_isSolid;
    }
    ShapeType::Enum type;
    int ctype;
    int x1;
    int y1;
    int x2;
    int y2;
    int extra1;
    int extra2;
    int color;
    bool isSolid;
  };
}
