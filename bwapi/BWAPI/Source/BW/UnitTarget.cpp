#include "UnitTarget.h"

#include "Offsets.h"

#include <BWAPI/UnitImpl.h>

namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  UnitTarget::UnitTarget()
      : targetID(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
#pragma warning(push)
#pragma warning(disable:4311)
  UnitTarget::UnitTarget(BWAPI::UnitImpl* target)
  {
    this->targetID = (int)((int)target->getOriginalRawData() - (int)BWDATA_UnitNodeTable) / 336 | 1 << 11;
  }
  //-------------------------------------------------- GETTARGET ---------------------------------------------
  u16 UnitTarget::getTarget() const
  {
    return this->targetID;
  }
#pragma warning(pop)
  //----------------------------------------------------------------------------------------------------------

}
