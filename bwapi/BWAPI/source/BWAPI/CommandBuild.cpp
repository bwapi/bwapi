#include "CommandBuild.h"
#include "Unit.h"
#include "Player.h"
#include "../BW/Unit.h"
#include "../../../Util/Logger.h"
namespace BWAPI
{
  //----------------------------- CONSTRUCTOR -----------------------------------
  CommandBuild::CommandBuild(Unit* builder, BW::UnitType toBuild, BW::Position position)
  :Command(builder)
  ,toBuild(toBuild)
  ,position(position)
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
      if (this->executors[0]->getType().isZerg())
        this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::DroneBuild;
      else if (this->executors[0]->getType().isProtoss())
        this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::BuildProtoss1;
      else if (this->executors[0]->getType().isTerran())
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
    return this->executors[0]->getName() + " will construct (" + toBuild.getName() + ")";
  }
  //-----------------------------------------------------------------------------
}