#include "UnitArray.h"
#include "Offsets.h"

namespace BW
{
  UnitArray::getIndexByUnit(Unit* unit)
  {
    return (int)(((u32)unit - (u32)BWDATA_UnitNodeTable) / sizeof(Unit));
  }
}
