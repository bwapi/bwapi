#include "TaskInvent.h"

#include "Globals.h"
#include <Util/Logger.h>
#include <BWAI/Player.h>

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskInvent::TaskInvent(BW::TechType techType, u16 priority)
  :Task(priority)
  ,techType(techType)
  {
    BWAI::ai->prioritisedTasks.insert(this);
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  TaskInvent::~TaskInvent()
  {
    BWAI::ai->prioritisedTasks.erase(this);
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  bool TaskInvent::execute()
  {
    if (BWAI::ai->player->techResearched(this->techType))
      return true;
    if (BWAI::ai->player->researchInProgress(this->techType))
      return false;
    if (this->executors.empty())
      {
        BW::UnitType buildingType = this->techType.whereToInvent();
        if (buildingType == BW::UnitID::None)
        {
          BWAI::ai->log->log("ERROR: Couldn't resolve where to invent %s", this->techType.getName());
          return false;
        }
        for each (Unit* i in BWAI::ai->units)
          if (i->isCompleted() &&
              i->getOrderID() == BW::OrderID::Nothing2 &&
              i->getType() == buildingType &&
              i->getTask() == NULL)
            this->addExecutor(i);
      }
    if (this->executors.empty())
      return false;
    for each (Unit* i in this->executors)
      if (i->isCompleted() &&
          i->getOrderID() == BW::OrderID::Nothing2 &&
          i->getOwner()->canAfford(this->techType))
      {
        this->executors.front()->invent(this->techType);
        break;
      }
    return false;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  TaskType::Enum TaskInvent::getType()
  {
    return TaskType::Invent;
  }
  //---------------------------------------------- GET MINERAL -----------------------------------------------
  BW::TechType TaskInvent::getTechType()
  {
    return this->techType;
  }
  //----------------------------------------------------------------------------------------------------------
}