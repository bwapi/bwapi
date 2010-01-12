#include "UnitID.h"

namespace BW
{
  //--------------------------------- CONSTRUCTOR -----------------------------------------
  UnitID::UnitID()
  {
  }
  //--------------------------------- CONSTRUCTOR -----------------------------------------
  UnitID::UnitID(int fromIndex)
    : bwId((u16)fromIndex + (1 << 11) + 1)
  {
  }
  //--------------------------------- GET INDEX -------------------------------------------
  int UnitID::getIndex() const
  {
    return (bwId & 0x7FF) - 1;
  }
}
