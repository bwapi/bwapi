#pragma once

namespace BW { class Unit; };
namespace BW /** @todo Investigate*/
{
#pragma pack(1)
  struct Order
  {
    BW::Order*   previousOrder;
    BW::Order*   nextOrder;
    u16          orderID;
    u16          unitType;
    BW::Position targetPosition;
    BW::Unit*    targetUnit;
  };
#pragma pack()
};
