#include <Util/Types.h>
#include <BWAPI/UnitType.h>

#include "Unit.h"

namespace BW
{
  BWAPI::UnitType Unit::type()
  {
    return BWAPI::UnitType(this->unitType);
  }
  bool Unit::movementFlag(u8 flags)
  {
    return (this->movementFlags & flags) != 0;
  }
  bool Unit::statusFlag(u32 flags)
  {
    return (this->statusFlags & flags) != 0;
  }
}
