#include "CommandTrain.h"
#include "Unit.h"
#include "Player.h"
#include "UnitPrototype.h"
#include "../BW//UnitData.h"
namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  CommandTrain::CommandTrain(Unit* building, UnitPrototype* toTrain)
  :Command(building)
  ,toTrain(toTrain)
  {
    this->execute();
  }
  //------------------------------ DESTRUCTOR -----------------------------------
  CommandTrain::~CommandTrain()
  {
  }
  //-------------------------------- EXECUTE ------------------------------------
  void CommandTrain::execute()
  {
   int slotToAffect = this->executors[0]->getQueueSlotLocal();
   if (this->executors[0]->getQueueLocal()[slotToAffect] != BW::UnitType::None)
      slotToAffect  = (slotToAffect + 1) % 5;
   
   if (this->executors[0]->getQueueLocal()[slotToAffect] != BW::UnitType::None)
   {
     this->failed = true;
     return;
   }
 
   executors[0]->getQueueLocal()[slotToAffect] = this->toTrain->getUnitID();
   this->executors[0]->getOwner()->spendLocal(toTrain->getMineralPrice(),
                                              toTrain->getGasPrice());
   this->executors[0]->getRawDataLocal()->queueSlot = slotToAffect;
    
  }
  //--------------------------------- GET TYPE ----------------------------------
  BWAPI::CommandTypes::Enum CommandTrain::getType()
  {
    return CommandTypes::Train;
  }
  //-----------------------------------------------------------------------------
}