#include "CommandCancelTrain.h"
#include "Unit.h"
#include "Player.h"
#include "UnitPrototype.h"
#include "../BW//UnitData.h"
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
   this->executors[0]->getBuildQueueLocal()[this->executors[0]->getBuildQueueSlotLocal()] = BW::UnitType::None;
   this->executors[0]->getRawDataLocal()->buildQueueSlot =
    (this->executors[0]->getRawDataLocal()->buildQueueSlot + 1) % 5;
  }
  //--------------------------------- GET TYPE ----------------------------------
  BWAPI::CommandTypes::Enum CommandCancelTrain::getType()
  {
    return CommandTypes::Train;
  }
  //-----------------------------------------------------------------------------
}