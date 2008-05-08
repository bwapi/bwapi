#include "Offsets.h"
#include "UnitTarget.h"
#include "..//BWAPI//Unit.h"
namespace BW
{
  #pragma warning(push)
  #pragma warning(disable:4311)
  UnitTarget::UnitTarget(BWAPI::Unit *target)
  {
    if (target == NULL)
      this->targetID = 0;
    else
      this->targetID = (int)((int)target->getOriginalRawData() - (int)UnitNodeTable)/336 | 1 << 11; 
  };
  #pragma warning(pop)
}
