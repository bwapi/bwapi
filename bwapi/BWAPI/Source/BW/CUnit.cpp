#include <Util/Types.h>
#include <BWAPI/UnitType.h>

#include "CUnit.h"

#include "../../../Debug.h"

namespace BW
{
  BWAPI::UnitType CUnit::type() const
  {
    return BWAPI::UnitType(this->unitType);
  }
  bool CUnit::movementFlag(u8 flags) const
  {
    return (this->movementFlags & flags) != 0;
  }
  bool CUnit::statusFlag(u32 flags) const
  {
    return (this->statusFlags & flags) != 0;
  }
}
