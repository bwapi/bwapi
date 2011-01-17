#include "UnitTarget.h"

#include "Offsets.h"

#include <BWAPI/UnitImpl.h>
#include <BW/Unit.h>

#include "../../Debug.h"

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitTarget::UnitTarget()
      : targetID(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitTarget::UnitTarget(BWAPI::UnitImpl* target)
  {
    u16 unitID = (u16)( ((u32)target->getOriginalRawData - (u32)BWDATA_UnitNodeTable) / 336 + 1);
    if (unitID <= UNIT_ARRAY_MAX_LENGTH)
      this->targetID = unitID | (target->getOriginalRawData->targetOrderSpecial << 11);
    else
      this->targetID = 0;
  }
  UnitTarget::UnitTarget(BW::Unit* target)
  {
    u16 unitID = (u16)( ((u32)target - (u32)BWDATA_UnitNodeTable) / 336 + 1);
    if (unitID <= UNIT_ARRAY_MAX_LENGTH)
      this->targetID = unitID | (target->targetOrderSpecial << 11);
    else
      this->targetID = 0;
  }
  //-------------------------------------------------- GETTARGET ---------------------------------------------
  u16 UnitTarget::getTarget() const
  {
    return this->targetID;
  }
  //----------------------------------------------------------------------------------------------------------

}
