#include "Unit.h"
#include "..//..//BWAPI//Source//Types.h"
#include "..//..//BWAPI//Source//BW//Offsets.h"
#include "Globals.h"
namespace BWAI
{
  //-------------------------------- CONSTRUCTOR ------------------------------
  Unit::Unit(BWAPI::Unit *unit)
  :BWAPI::Unit(*unit) /**< default autoconstructor usage */
  ,expansionAssingment(0)
  ,selected(false)
  ,lastTrainedUnitID(BW::UnitType::None)
  {
  }
  //----------------------------- BW Unit TO BWAI Unit -----------------------
  #pragma warning(push)
  #pragma warning(disable:4311)
  Unit* Unit::BWUnitToBWAIUnit(BW::UnitData* unit)
  {
    if (unit == NULL)
      return NULL;
    return ai.getUnit(((int)unit - (int)BW::BWXFN_UnitNodeTable)/336);
  }
  #pragma warning (pop)
  //---------------------------------------------------------------------------
}