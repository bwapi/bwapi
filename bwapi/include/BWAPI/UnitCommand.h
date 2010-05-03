#pragma once

#include "UnitCommandType.h"

namespace BWAPI
{
  class Unit;
  class UnitCommand
  {
    public:
      UnitCommand(UnitCommandType::Enum _type, Unit* _target, int _x, int _y, int _extra) : type(_type), target(_target), x(_x), y(_y), extra(_extra) {}
      UnitCommandType::Enum type;
      Unit* target;
      int x;
      int y;
      int extra;
  };
}