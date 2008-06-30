#include "TaskUpgrade.h"

#include "Globals.h"
#include <Util/Logger.h>
#include <BWAPI/Player.h>
#include <BuildOrder/Root.h>

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskUpgrade::TaskUpgrade(BW::UpgradeType upgradeType, u8 level)
  :Task()
  ,upgradeType(upgradeType)
  ,level(level)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskUpgrade::~TaskUpgrade()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
  bool TaskUpgrade::execute()
  {
    if (BWAI::ai->player->upgradeLevel(this->upgradeType) >= this->level)
    {
      BWAI::ai->root->log->log("Upgrade %s finished", this->getUpgradeType().getName());
      return true;
    }
    if (BWAI::ai->player->upgradeInProgress(this->upgradeType))
      return false;
    if (this->executors.empty())
      {
        BW::UnitType buildingType = this->upgradeType.whereToUpgrade();
        if (buildingType == BW::UnitID::None)
        {
          BWAI::ai->log->log("ERROR: Couldn't resolve where to upgrade %s", this->upgradeType.getName());
          return false;
        }
        for (Unit* i = BWAI::ai->getFirst(); i != NULL; i = i->getNext())
          if (i->getType() == buildingType)
            this->addExecutor(i);
      }
    if (this->executors.empty())
      return false;
    for each (Unit* i in this->executors)
      if (i->isReady() &&
          i->getOrderID() == BW::OrderID::Nothing2 &&
          i->getOwner()->canAfford(this->upgradeType, this->level, BWAPI::ReservedResources()))
      {
        this->executors.front()->upgrade(this->upgradeType);
        break;
      }
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskUpgrade::getType()
  {
    return TaskType::Invent;
  }
  //------------------------------- GET MINERAL -------------------------------
  BW::UpgradeType TaskUpgrade::getUpgradeType()
  {
    return this->upgradeType;
  }
  //---------------------------------------------------------------------------
}