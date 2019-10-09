#pragma once
#include <BWAPI/Client/ShapeType.h>
#include <BWAPI/CoordinateType.h>

namespace BWAPIC4
{
  struct Shape
  {
    Shape()
    :type(BWAPI::ShapeType::None)
    ,ctype(BWAPI::CoordinateType::None)
    ,x1(0)
    ,y1(0)
    ,x2(0)
    ,y2(0)
    ,extra1(0)
    ,extra2(0)
    ,color(0)
    ,isSolid(false)
    {
    }

    Shape(BWAPI::ShapeType::Enum _shapeType, BWAPI::CoordinateType::Enum _ctype, int _x1, int _y1, int _x2, int _y2, int _extra1, int _extra2, int _color, bool _isSolid, const std::string& _text = {})
    :type(_shapeType)
    ,ctype(_ctype)
    ,x1(_x1)
    ,y1(_y1)
    ,x2(_x2)
    ,y2(_y2)
    ,extra1(_extra1)
    ,extra2(_extra2)
    ,color(_color)
    ,isSolid(_isSolid)
    ,text(_text)
    {
    }

    BWAPI::ShapeType::Enum type;
    BWAPI::CoordinateType::Enum ctype;
    int x1;
    int y1;
    int x2;
    int y2;
    int extra1;
    int extra2;
    int color;
    bool isSolid;
    std::string text;
  };
}
