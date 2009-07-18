#include "CommandBuild.h"

#include <Util/Logger.h>

#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandBuild::CommandBuild(UnitImpl* builder, BW::UnitType toBuild, BW::TilePosition position)
      : Command(builder)
      , toBuild(toBuild)
      , position(position)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandBuild::~CommandBuild()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandBuild::execute()
  {
    if (this->executors[0]->isReady())
      switch (this->executors[0]->getType().getID())
      {
        case BW::UnitID::Zerg_Drone    : this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::DroneBuild; break;
        case BW::UnitID::Protoss_Probe : this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::BuildProtoss1; break;
        case BW::UnitID::Terran_SCV    : this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::BuildTerran; break;
        case BW::UnitID::Terran_CommandCenter :
        case BW::UnitID::Terran_Factory :
        case BW::UnitID::Terran_Starport :
        case BW::UnitID::Terran_ScienceFacility : this->executors[0]->getRawDataLocal()->secondaryOrderID = BW::OrderID::BuildAddon; break;
      }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandBuild::getType()
  {
    return CommandTypes::Build;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandBuild::describe()
  {
    return this->executors[0]->getName() + " will construct (" + toBuild.getName() + ")";
  }
  //----------------------------------------------------------------------------------------------------------
}