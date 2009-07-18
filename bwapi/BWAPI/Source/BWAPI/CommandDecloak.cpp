#include "CommandDecloak.h"
#include "UnitImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandDecloak::CommandDecloak(UnitImpl* executor)
      : Command(executor)
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandDecloak::execute()
  {
    for (unsigned int i = 0; i < this->executors.size(); i++)
    {
      if ((this->executors[i]->getType().canMove()))
      {
        this->executors[i]->getRawDataLocal()->orderID = BW::OrderID::Decloak;
      }
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandDecloak::getType()
  {
    return BWAPI::CommandTypes::Decloak;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandDecloak::describe()
  {
    return this->executors[0]->getName() + " decloaked";
  }
  //----------------------------------------------------------------------------------------------------------
}