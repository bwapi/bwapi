#include "CommandUpgrade.h"

#include <Util/Logger.h>

#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandUpgrade::CommandUpgrade(UnitImpl* building, BW::UpgradeType upgrade)
      : Command(building)
      , upgrade(upgrade)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandUpgrade::~CommandUpgrade()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandUpgrade::execute()
  {
    if (!this->executors[0]->_exists()) return;
    if (this->executors[0]->isCompleted())
    {
      this->executors[0]->getRawDataLocal()->orderID = BW::OrderID::Upgrade;
      this->executors[0]->getRawDataLocal()->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion2.upgradeID = upgrade.getID();
      this->executors[0]->getRawDataLocal()->childUnitUnion1.unitIsBuilding.upgradeResearchTime = (u16)(upgrade.timeCostBase()+upgrade.timeCostFactor()*this->executors[0]->getPlayer()->getUpgradeLevel(upgrade.getID()));
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandUpgrade::getType()
  {
    return CommandTypes::Upgrade;
  }
  //----------------------------------------------------------------------------------------------------------
};
