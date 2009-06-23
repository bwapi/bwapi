#include "TaskInvent.h"

#include "Globals.h"
#include <Util/Logger.h>
#include <BWAI/Player.h>

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TaskInvent::TaskInvent(BWAPI::TechType techType, u16 priority)
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
        BWAPI::UnitType buildingType = *(this->techType.whatResearches());
        if (buildingType == BWAPI::UnitTypes::None)
        {
          BWAI::ai->log->log("ERROR: Couldn't resolve where to invent %s", this->techType.getName().c_str());
          return false;
        }
        for each (Unit* i in BWAI::ai->units)
          if (i->isCompleted() &&
              i->getOrder() == BWAPI::Orders::Nothing2 &&
              i->getType() == buildingType &&
              i->getTask() == NULL)
            this->addExecutor(i);
      }
    if (this->executors.empty())
      return false;
    for each (Unit* i in this->executors)
      if (i->isCompleted() &&
          i->getOrder() == BWAPI::Orders::Nothing2 &&
          i->getOwner()->canAfford(this->techType))
      {
        this->executors.front()->research(this->techType);
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
  BWAPI::TechType TaskInvent::getTechType()
  {
    return this->techType;
  }
  //----------------------------------------------------------------------------------------------------------
}