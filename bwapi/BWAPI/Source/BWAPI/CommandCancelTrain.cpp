#include "CommandCancelTrain.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelTrain::CommandCancelTrain(UnitImpl* building)
  :Command(building)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandCancelTrain::~CommandCancelTrain()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelTrain::execute()
  {
   this->executors[0]->getBuildQueue()[this->executors[0]->getBuildQueueSlot()] = BW::UnitID::None;
   this->executors[0]->getRawDataLocal()->buildQueueSlot =
    (this->executors[0]->getRawDataLocal()->buildQueueSlot + 1) % 5;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandCancelTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCancelTrain::describe()
  {
    return this->executors[0]->getName() + " canceled unit production";
  }
  //----------------------------------------------------------------------------------------------------------
}