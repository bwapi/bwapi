#include "CommandTrain.h"
#include "UnitImpl.h"
#include "GameImpl.h"
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
    startFrame = Broodwar->getFrameCount();
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandTrain::~CommandTrain()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandTrain::execute()
  {
    if (!executors[0]->_exists) return;
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
    {
      executors[0]->self->trainingQueue[executors[0]->self->trainingQueueCount] = toTrain.getID();
      executors[0]->self->trainingQueueCount++;
      p->spend(toTrain.mineralPrice(), toTrain.gasPrice());
      p->planToMake(toTrain);
    }
    executors[0]->self->remainingTrainTime = UnitType(toTrain.getID()).buildTime();
    executors[0]->self->isTraining = true;
    executors[0]->self->isIdle = false;
    if (toTrain == BW::UnitID::Terran_NuclearMissile)
      this->executors[0]->self->secondaryOrder = BW::OrderID::Train;
    p->useSupplies(toTrain.supplyRequired(), toTrain._getRace());
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
};
