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
    if (!executors[0]->_exists) return;
    executors[0]->self->trainingQueue[executors[0]->self->trainingQueueCount] = toTrain.getID();
    executors[0]->self->trainingQueueCount++;
    executors[0]->self->remainingTrainTime = UnitType(toTrain.getID()).buildTime();
    executors[0]->self->isTraining = true;
    executors[0]->self->isIdle = false;
    if (toTrain == BW::UnitID::Terran_NuclearMissile)
      this->executors[0]->self->secondaryOrder = BW::OrderID::Train;
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    p->spend(toTrain.mineralPrice(), toTrain.gasPrice());
    p->useSupplies(toTrain.supplyRequired(), toTrain._getRace());
    p->planToMake(toTrain);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
};
