#include "TaskUpgrade.h"

#include "Globals.h"
#include <Util/Logger.h>
#include <BWAI/Player.h>
#include <BuildOrder/Root.h>

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskUpgrade::TaskUpgrade(BWAPI::UpgradeType upgradeType, u8 level, u16 priority)
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
      BWAI::ai->root->log->log("Upgrade %s finished", this->getUpgradeType().getName().c_str());
      return true;
    }
    if (BWAI::ai->player->upgradeInProgress(this->upgradeType))
      return false;
    if (this->executors.empty())
    {
      BWAPI::UnitType buildingType = *(this->upgradeType.whatUpgrades());
      if (buildingType == BWAPI::UnitTypes::None)
      {
        BWAI::ai->log->log("ERROR: Couldn't resolve where to upgrade %s", this->upgradeType.getName().c_str());
        return false;
      }
      for each (Unit* i in BWAI::ai->units)
        if (i->isCompleted() &&
            i->getOrderID() == BWAPI::Orders::Nothing2 &&
            i->getType() == buildingType)
          this->addExecutor(i);
    }
    if (this->executors.empty())
      return false;
    for each (Unit* i in this->executors)
      if (i->isCompleted() &&
          i->getOrderID() == BWAPI::Orders::Nothing2 &&
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
  //-------------------------------------------- GET UPGRADE TYPE --------------------------------------------
  BWAPI::UpgradeType TaskUpgrade::getUpgradeType()
  {
    return this->upgradeType;
  }
  //----------------------------------------------------------------------------------------------------------
}