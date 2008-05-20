#include "CommandCancelTrain.h"
#include "Unit.h"
#include "Player.h"
#include "../BW//Unit.h"
namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  CommandCancelTrain::CommandCancelTrain(Unit* building)
  :Command(building)
  {
  }
  //------------------------------ DESTRUCTOR -----------------------------------
  CommandCancelTrain::~CommandCancelTrain()
  {
  }
  //-------------------------------- EXECUTE ------------------------------------
  void CommandCancelTrain::execute()
  {
   this->executors[0]->getBuildQueueLocal()[this->executors[0]->getBuildQueueSlotLocal()] = BW::UnitID::None;
   this->executors[0]->getRawDataLocal()->buildQueueSlot =
    (this->executors[0]->getRawDataLocal()->buildQueueSlot + 1) % 5;
  }
  //--------------------------------- GET TYPE ----------------------------------
  BWAPI::CommandTypes::Enum CommandCancelTrain::getType()
  {
    return CommandTypes::Train;
  }
  //-----------------------------------------------------------------------------
  std::string CommandCancelTrain::describe()
  {
    return this->executors[0]->getName() + " canceled unit production";
  }
  //-----------------------------------------------------------------------------
}