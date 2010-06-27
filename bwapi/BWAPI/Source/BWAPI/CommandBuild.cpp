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
    if (!executors[0]->_exists) return;
    switch (executors[0]->getType().getID())
    {
      case BW::UnitID::Zerg_Drone    : executors[0]->self->order = BW::OrderID::DroneLand; break;
      case BW::UnitID::Protoss_Probe : executors[0]->self->order = BW::OrderID::BuildProtoss1; break;
      case BW::UnitID::Terran_SCV    : executors[0]->self->order = BW::OrderID::BuildTerran; break;
      case BW::UnitID::Terran_CommandCenter :
      case BW::UnitID::Terran_Factory :
      case BW::UnitID::Terran_Starport :
      case BW::UnitID::Terran_ScienceFacility : executors[0]->self->secondaryOrder = BW::OrderID::BuildAddon; break;
    }
    executors[0]->self->isConstructing = true;
    executors[0]->self->isIdle         = false;
    executors[0]->self->buildType      = toBuild.getID();
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandBuild::getType()
  {
    return CommandTypes::Build;
  }
  //----------------------------------------------------------------------------------------------------------
};
