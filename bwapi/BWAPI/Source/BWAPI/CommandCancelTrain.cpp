#include "CommandCancelTrain.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelTrain::CommandCancelTrain(UnitImpl* building)
      : Command(building)
      , slot(-2)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelTrain::CommandCancelTrain(UnitImpl* building, int slot)
      : Command(building)
      , slot(slot)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandCancelTrain::~CommandCancelTrain()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelTrain::execute()
  {
    if (!this->executors[0]->_exists()) return;
    if (slot < 0)
    {
      int i = this->executors[0]->getBuildQueueSlot() % 5;
      int starti = i;
      while(this->executors[0]->getBuildQueue()[(i+1)%5] != BW::UnitID::None && (i + 1) % 5 != starti)
      {
        i = (i + 1) % 5;
      }
      this->executors[0]->getBuildQueue()[i] = BW::UnitID::None;
    }
    else
    {
      int i = this->executors[0]->getBuildQueueSlot() % 5;
      int starti = i;
      for(int j = 0; j < slot; j++)
      {
        i = (i + 1) % 5;
      }
      while(this->executors[0]->getBuildQueue()[(i+1)%5] != BW::UnitID::None && (i + 1) % 5 != starti)
      {
        this->executors[0]->getBuildQueue()[i] = this->executors[0]->getBuildQueue()[(i+1)%5];
        i = (i + 1) % 5;
      }
      this->executors[0]->getBuildQueue()[i] = BW::UnitID::None;
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelTrain::getType()
  {
    return CommandTypes::Train;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandCancelTrain::describe()
  {
    return this->executors[0]->getName() + " cancelled unit production";
  }
  //----------------------------------------------------------------------------------------------------------
};
