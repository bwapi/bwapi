#include "TaskUpgrade.h"

#include "Globals.h"
#include <Util/Logger.h>
#include <BWAI/Player.h>
#include <BuildOrder/Root.h>

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskUpgrade::TaskUpgrade(BW::UpgradeType upgradeType, u8 level, u16 priority)
  :Task(priority)
  ,upgradeType(upgradeType)
  ,level(level)
  {
    BWAI::ai->prioritisedTasks.insert(this);
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  TaskUpgrade::~TaskUpgrade()
  {
    BWAI::ai->prioritisedTasks.erase(this);
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
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
      for each (Unit* i in BWAI::ai->units)
        if (i->isReady() &&
            i->getOrderID() == BW::OrderID::Nothing2 &&
            i->getType() == buildingType)
          this->addExecutor(i);
    }
    if (this->executors.empty())
      return false;
    for each (Unit* i in this->executors)
      if (i->isReady() &&
          i->getOrderID() == BW::OrderID::Nothing2 &&
          i->getOwner()->canAfford(this->upgradeType, this->level))
      {
        this->executors.front()->upgrade(this->upgradeType);
        break;
      }
    return false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  TaskType::Enum TaskUpgrade::getType()
  {
    return TaskType::Invent;
  }
  //---------------------------------------------- GET MINERAL -----------------------------------------------
  BW::UpgradeType TaskUpgrade::getUpgradeType()
  {
    return this->upgradeType;
  }
  //----------------------------------------------------------------------------------------------------------
}