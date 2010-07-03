#include "CommandCancelUpgrade.h"
#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandCancelUpgrade::CommandCancelUpgrade(UnitImpl* executor)
      : Command(executor)
  {
    startFrame = Broodwar->getFrameCount();
    upgradeType = executor->getUpgrade();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandCancelUpgrade::execute()
  {
    if (!executors[0]->_exists) return;
    UpgradeType upgrade = UpgradeType(executors[0]->self->upgrade);
    executors[0]->self->order = BW::OrderID::Nothing;
    int level = this->executors[0]->getPlayer()->getUpgradeLevel(executors[0]->self->upgrade);
    executors[0]->self->upgrade = UpgradeTypes::None.getID();
    executors[0]->self->isIdle = true;
    executors[0]->self->remainingUpgradeTime = 0;
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      p->spend(-upgrade.mineralPriceBase()-upgrade.mineralPriceFactor()*level, -upgrade.gasPriceBase()-upgrade.gasPriceFactor()*level);
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandCancelUpgrade::getType()
  {
    return BWAPI::CommandTypes::CancelUpgrade;
  }
  //----------------------------------------------------------------------------------------------------------
};
