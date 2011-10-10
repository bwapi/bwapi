#include <Util/Types.h>
#include <BWAPI/UnitType.h>

#include "Unit.h"

#include "../../Debug.h"

namespace BW
{
  BWAPI::UnitType Unit::type() const
  {
    return BWAPI::UnitType(this->unitType);
  }
  bool Unit::movementFlag(u8 flags) const
  {
    return (this->movementFlags & flags) != 0;
  }
  bool Unit::statusFlag(u32 flags) const
  {
    return (this->statusFlags & flags) != 0;
  }
}
