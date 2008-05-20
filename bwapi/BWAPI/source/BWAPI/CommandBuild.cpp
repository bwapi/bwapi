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
      if (this->executors[0]->getPrototype()->isZerg())
        this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::DroneBuild;
      else if (this->executors[0]->getPrototype()->isProtoss())
        this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::BuildProtoss1;
      else if (this->executors[0]->getPrototype()->isTerran())
        this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::BuildTerran;
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