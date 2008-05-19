#include "CommandBuild.h"
#include "Unit.h"
#include "Player.h"
#include "UnitPrototype.h"
#include "../BW/Unit.h"
#include "../../../Util/Logger.h"
namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  CommandBuild::CommandBuild(Unit* builder, UnitPrototype* toBuild)
  :Command(builder)
  ,toBuild(toBuild)
  {
  }
  //------------------------------ DESTRUCTOR -----------------------------------
  CommandBuild::~CommandBuild()
  {
  }
  //-------------------------------- EXECUTE ------------------------------------
  void CommandBuild::execute()
  {
    if (this->executors[0]->isReady())
      this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::GoingToBuild;
  }
  //--------------------------------- GET TYPE ----------------------------------
  BWAPI::CommandTypes::Enum CommandBuild::getType()
  {
    return CommandTypes::Build;
  }
  //-----------------------------------------------------------------------------
  std::string CommandBuild::describe()
  {
    return this->executors[0]->getName() + " will construct (" + toBuild->getName() + ")";
  }
  //-----------------------------------------------------------------------------
}