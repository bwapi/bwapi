#include "CommandCancelTrain.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelTrain::CommandCancelTrain(UnitImpl* building)
      : Command(building)
      , slot(-1)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelTrain::CommandCancelTrain(UnitImpl* building, int slot)
      : Command(building)
      , slot(slot)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandCancelTrain::~CommandCancelTrain()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelTrain::execute()
  {
    if (!executors[0]->_exists) return;
    if (slot >= 0)
    {
      for(int i=slot;i<4;i++)
        executors[0]->self->trainingQueue[i]=executors[0]->self->trainingQueue[i+1];
    }
    executors[0]->self->trainingQueueCount--;
    if (executors[0]->self->trainingQueueCount<0)
      executors[0]->self->trainingQueueCount=0;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
};
