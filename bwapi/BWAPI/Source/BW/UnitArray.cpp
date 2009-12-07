#include "UnitArray.h"
#include "Offsets.h"

#include <Util\Types.h>

namespace BW
{
  int UnitArray::getIndexByUnit(Unit* unit)
  {
    return (int)(((u32)unit - (u32)BWDATA_UnitNodeTable) / sizeof(Unit));
  }
}
