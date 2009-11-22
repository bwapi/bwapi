#include "CommandUnloadAll.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnloadAll::CommandUnloadAll(UnitImpl* executor)
      : Command(executor)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUnloadAll::CommandUnloadAll(UnitImpl* executor, BW::Position targetPosition)
      : Command(executor)
      , targetPosition(targetPosition)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUnloadAll::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if ((this->executors[i]->getType().canMove()))
      {
        if (this->executors[i]->getType() == UnitTypes::Terran_Bunker)
        {
          this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Unload;
        }
        else
        {
          this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::MoveUnload;
          this->executors[i]->getRawDataLocal()->position = this->targetPosition;
        }
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUnloadAll::getType()
  {
    return BWAPI::CommandTypes::UnloadAll;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandUnloadAll::describe()
  {
    return this->executors[0]->getName() + " unloaded all units";
  }
  //----------------------------------------------------------------------------------------------------------
}