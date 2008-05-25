#include "Offsets.h"
#include "UnitTarget.h"
#include "..//BWAPI//Unit.h"
namespace BW
{
  
  //--------------------------------- CONSTRUCTOR -----------------------------
  UnitTarget::UnitTarget()
  :targetID(0)
  {
  }
  //--------------------------------- CONSTRUCTOR -----------------------------
  #pragma warning(push)
  #pragma warning(disable:4311)
  UnitTarget::UnitTarget(BWAPI::Unit *target)
  {
    this->targetID = (int)((int)target->getOriginalRawData() - (int)BWXFN_UnitNodeTable)/336 | 1 << 11; 
  }
  #pragma warning(pop)
  //---------------------------------------------------------------------------
  
}
