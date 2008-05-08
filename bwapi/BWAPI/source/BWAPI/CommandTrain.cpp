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
   int slotToAffect = this->executors[0]->getBuildQueueSlotLocal();
   if (this->executors[0]->getBuildQueueLocal()[slotToAffect] != BW::UnitType::None)
      slotToAffect  = (slotToAffect + 1) % 5;
   
   if (this->executors[0]->getBuildQueueLocal()[slotToAffect] != BW::UnitType::None)
   {
     this->failed = true;
     return;
   }
 
   executors[0]->getBuildQueueLocal()[slotToAffect] = this->toTrain->getUnitID();
   this->executors[0]->getOwner()->spendLocal(toTrain->getMineralPrice(),
                                              toTrain->getGasPrice());
   this->executors[0]->getRawDataLocal()->buildQueueSlot = slotToAffect;
   switch (toTrain->getRace())
   {
     case BWAPI::RaceType::Protoss : executors[0]->getOwner()->useSuppliesProtossLocal(toTrain->getSupplies()); break;
     case BWAPI::RaceType::Terran  : executors[0]->getOwner()->useSuppliesTerranLocal (toTrain->getSupplies()); break;
     case BWAPI::RaceType::Zerg    : executors[0]->getOwner()->useSuppliesZergLocal   (toTrain->getSupplies()); break;
   }
  }
  //--------------------------------- GET TYPE ----------------------------------
  BWAPI::CommandTypes::Enum CommandTrain::getType()
  {
    return CommandTypes::Train;
  }
  //-----------------------------------------------------------------------------
}