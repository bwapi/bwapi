#include "TaskUpgrade.h"

#include "Globals.h"
#include <Util/Logger.h>
#include <BWAPI/Player.h>

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
      return true;
    if (BWAI::ai->player->upgradeInProgress(this->upgradeType))
      return false;
    if (this->executors.empty())
      {
        BW::UnitType buildingType = this->upgradeType.whereToUpgrade();
        {
          BWAI::ai->log->log("ERROR: Couldn't resolve where to upgrade %s", this->upgradeType.getName());
          return false;
        }
        for (Unit* i = BWAI::ai->getFirst(); i != NULL; i = i->getNext())
          if (i->getType() == buildingType &&
              i->getTask() == NULL)
            this->addExecutor(i);
      }
    if (this->executors.empty())
      return false;
    for (std::list<Unit*>::iterator i = this->executors.begin();
         i != this->executors.end();
         ++i)
      if ((*i)->isReady() &&
          (*i)->getOrderID() == BW::OrderID::Nothing2 &&
          (*i)->getOwner()->canAfford(this->upgradeType, level))
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