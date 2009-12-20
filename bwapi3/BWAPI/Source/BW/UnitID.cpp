#include "UnitID.h"

namespace BW
{
  //--------------------------------- CONSTRUCTOR -----------------------------------------
  UnitID::UnitID()
  {
  }
  //--------------------------------- CONSTRUCTOR -----------------------------------------
  UnitID::UnitID(int fromIndex)
    : id((u16)fromIndex + (1 << 11))
  {
  }
  //--------------------------------- GET INDEX -------------------------------------------
  int UnitID::getIndex() const
  {
    return (id & 0x7FF) - 1;
  }
}
