#include "CommandCancelConstruction.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelConstruction::CommandCancelConstruction(UnitImpl* executor)
      : Command(executor)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelConstruction::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Nothing;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelConstruction::getType()
  {
    return BWAPI::CommandTypes::CancelResearch;
  }
  //----------------------------------------------------------------------------------------------------------
};
