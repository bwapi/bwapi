#pragma once

namespace BW { struct Unit; };
#include "UnitType.h"
namespace BW /** @todo Investigate*/
{
#pragma pack(1)
  struct Order
  {
    BW::Order*   previousOrder;
    BW::Order*   nextOrder;
    u16          orderID;
    UnitType     unitType;
    BW::Position targetPosition;
    BW::Unit*    targetUnit;
  };
#pragma pack()
};
