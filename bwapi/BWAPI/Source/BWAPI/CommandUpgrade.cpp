#include "CommandUpgrade.h"

#include <Util/Logger.h>

#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUpgrade::CommandUpgrade(UnitImpl* building, BW::UpgradeType upgrade)
      : Command(building)
      , upgrade(upgrade)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandUpgrade::~CommandUpgrade()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUpgrade::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::Upgrade;
    executors[0]->self->upgrade = upgrade.getID();
    executors[0]->self->isIdle = false;
    int level = this->executors[0]->getPlayer()->getUpgradeLevel(upgrade.getID());
    executors[0]->self->remainingUpgradeTime = upgrade.timeCostBase()+upgrade.timeCostFactor()*level;
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      p->spend(upgrade.mineralCostBase()+upgrade.mineralCostFactor()*level, upgrade.gasCostBase()+upgrade.gasCostFactor()*level);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUpgrade::getType()
  {
    return CommandTypes::Upgrade;
  }
  //----------------------------------------------------------------------------------------------------------
};
