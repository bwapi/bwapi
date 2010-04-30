#pragma once

#include "UnitCommandType.h"

namespace BWAPI
{
  class Unit;
  class UnitCommand
  {
    public:
      UnitCommand(UnitCommandType::Enum type, Unit* target, int x, int y, int extra) : type(type), target(target), x(x), y(y), extra(extra) {}
      UnitCommandType::Enum type;
      Unit* target;
      int x;
      int y;
      int extra;
  };
}