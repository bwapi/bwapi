#pragma once

namespace BW { struct Unit; };
namespace BW /** @todo Investigate*/
{
#pragma pack(1)
  struct Order
  {
    BW::Order*   previousOrder;
    BW::Order*   nextOrder;
    u16          orderID;
    u16          unknown_0xE4;
    BW::Position targetPosition;
    BW::Unit*    targetUnit;
  };
#pragma pack()
};
