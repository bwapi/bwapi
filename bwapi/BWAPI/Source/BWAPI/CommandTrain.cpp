#include "CommandTrain.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <Util/Logger.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandTrain::CommandTrain(UnitImpl* building, BW::UnitType toTrain)
      : Command(building)
      , toTrain(toTrain)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandTrain::~CommandTrain()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandTrain::execute()
  {
    if (toTrain == BW::UnitID::Terran_NuclearMissile)
    {
      this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::NukeTrain;
      return;
    }
    int slotToAffect = this->executors[0]->getBuildQueueSlot();
    if (this->executors[0]->getBuildQueue()[slotToAffect] != BW::UnitID::None)
      slotToAffect  = (slotToAffect + 1) % 5;

    if (this->executors[0]->getBuildQueue()[slotToAffect] != BW::UnitID::None)
    {
      this->failed = true;
      return;
    }

    executors[0]->getBuildQueue()[slotToAffect] = this->toTrain.getID();
    this->executors[0]->getRawDataLocal()->buildQueueSlot = slotToAffect;
    PlayerImpl* p = static_cast<PlayerImpl*>(this->executors[0]->getPlayer());
    p->spend(this->toTrain.mineralPrice(), this->toTrain.gasPrice());
    p->useSupplies(toTrain.supplyRequired(), toTrain.getRace());
    p->planToMake(toTrain);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandTrain::describe()
  {
    std::string toReturn = this->executors[0]->getName() + " started to build (" + toTrain.getName() + ")";
    return toReturn;
  }
  //----------------------------------------------------------------------------------------------------------
}