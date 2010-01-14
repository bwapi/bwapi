#include "UnitArray.h"
#include "Offsets.h"

#include <Util\Types.h>

namespace BW
{
  int UnitArray::getIndexByUnit(Unit* unit)
  {
    int offset = (u32)unit - (u32)BWDATA_UnitNodeTable;
    _ASSERT((offset % sizeof(Unit)) == 0);  // failfast
    return (int)(offset / sizeof(Unit));
  }
}
