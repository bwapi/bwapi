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
    if (!this->executors[0]->_exists) return;
    if (this->executors[0]->isCompleted())
    {
      switch (this->executors[0]->getType().getID())
      {
        case BW::UnitID::Zerg_Drone    : this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::DroneLand; break;
        case BW::UnitID::Protoss_Probe : this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::BuildProtoss1; break;
        case BW::UnitID::Terran_SCV    : this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::BuildTerran; break;
        case BW::UnitID::Terran_CommandCenter :
        case BW::UnitID::Terran_Factory :
        case BW::UnitID::Terran_Starport :
        case BW::UnitID::Terran_ScienceFacility : this->executors[0]->getRawDataLocal()->secondaryOrderID = BW::OrderID::BuildAddon; break;
      }
      int slotToAffect = this->executors[0]->getBuildQueueSlot();
      if (this->executors[0]->getBuildQueue()[slotToAffect] != BW::UnitID::None)
        slotToAffect  = (slotToAffect + 1) % 5;

      executors[0]->getBuildQueue()[slotToAffect] = this->toBuild.getID();
      this->executors[0]->getRawDataLocal()->buildQueueSlot = (u8)slotToAffect;
    }

  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandBuild::getType()
  {
    return CommandTypes::Build;
  }
  //----------------------------------------------------------------------------------------------------------
};
