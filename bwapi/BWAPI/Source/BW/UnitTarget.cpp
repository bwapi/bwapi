#include "UnitTarget.h"

#include "Offsets.h"

#include <BWAPI/UnitImpl.h>
#include <BW/CUnit.h>

#include "../../../Debug.h"

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
    u16 unitID = (u16)((target->getOriginalRawData - BWDATA::UnitNodeTable.data()) + 1);
    if (unitID <= UNIT_ARRAY_MAX_LENGTH)
      this->targetID = unitID | (target->getOriginalRawData->uniquenessIdentifier << 11);
    else
      this->targetID = 0;
  }
  UnitTarget::UnitTarget(BW::CUnit* target)
  {
    u16 unitID = (u16)((target - BWDATA::UnitNodeTable.data()) + 1);
    if (unitID <= UNIT_ARRAY_MAX_LENGTH)
      this->targetID = unitID | (target->uniquenessIdentifier << 11);
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
