#pragma once

#include "OrderTypeId.h"
#include "UnitId.h"
#include "Position.h"

namespace BWAPI
{
  struct Order
  {
    OrderTypeId type;
    UnitId targetUnit;
    Position targetPosition;
  };
}
