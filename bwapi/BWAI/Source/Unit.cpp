#include "Unit.h"
#include "..//..//BWAPI//Source//Types.h"
#include "..//..//BWAPI//Source//BW//Offsets.h"
#include "..//..//BWAPI//Source//BW//Unit.h"
#include "..//..//BWAPI//Source//BWAPI//Globals.h"
#include "Globals.h"
#include "..//..//BWAPI//Source//Logger.h"
namespace BWAI
{
  //-------------------------------- CONSTRUCTOR ------------------------------
  Unit::Unit(BWAPI::Unit *unit)
  :BWAPI::Unit(*unit) /**< default autoconstructor usage */
  ,expansionAssingment(0)
  ,selected(false)
  ,lastTrainedUnitID(BW::UnitType::None)
  ,lastFrameSpam(0)
  {
  }
  //----------------------------- BW Unit TO BWAI Unit -----------------------
  #pragma warning(push)
  #pragma warning(disable:4311)
  Unit* Unit::BWUnitToBWAIUnit(BW::Unit* unit)
  {
    if (unit == NULL)
      return NULL;
    return ai->getUnit(((int)unit - (int)BW::BWXFN_UnitNodeTable)/336);
  }
  #pragma warning (pop)
  //------------------------------- GET TARGET --------------------------------
  Unit* Unit::getTarget()
  {
    return BWAI::Unit::BWUnitToBWAIUnit(this->getOriginalRawData()->targetUnit);
  }
  //----------------------------- GET TARGET LOCAL --------------------------
  Unit* Unit::getTargetLocal()
  {
    return BWAI::Unit::BWUnitToBWAIUnit(this->getRawDataLocal()->targetUnit);
  }
  //--------------------------------- GET NEXT ---------------------------------
  Unit *Unit::getNext()
  {
    return this->next;
  }
  //-------------------------------- UPDATE NEXT -------------------------------
  void Unit::updateNext()
  {
    #pragma warning(push)
    #pragma warning(disable:4311)
    if (this->getOriginalRawData()->nextUnit != NULL)
    {
      if (((int)this->getOriginalRawData()->nextUnit - (int)BW::BWXFN_UnitNodeTable)/BW::UNIT_SIZE_IN_BYTES >= BW::UNIT_ARRAY_MAX_LENGTH)
        BWAPI::Broodwar.fatalError->log("Unit array too small, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
      if ((int)this->getOriginalRawData()->nextUnit < (int)BW::BWXFN_UnitNodeTable)
        BWAPI::Broodwar.fatalError->log("Unit array begins at bad location, found unit with addr %X", (int)this->getOriginalRawData()->nextUnit);
    }
    #pragma warning(push)
    this->next = Unit::BWUnitToBWAIUnit(this->getOriginalRawData()->nextUnit);
    if (this->next != NULL)
      this->next->updateNext();
  }
  //---------------------------------------------------------------------------
}