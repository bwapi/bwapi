#include "TaskInvent.h"

#include "Globals.h"
#include <Util/Logger.h>
#include <BWAPI/Player.h>

namespace BWAI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  TaskInvent::TaskInvent(BW::TechType techType)
  :Task()
  ,techType(techType)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  TaskInvent::~TaskInvent()
  {
  }
  //-------------------------------- EXECUTE ----------------------------------
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
          (*i)->getOwner()->canAfford(this->techType, BWAPI::ReservedResources()))
      {
        this->executors.front()->invent(this->techType);
        break;
      }
    return false;
  }
  //-------------------------------- GET TYPE ---------------------------------
  TaskType::Enum TaskInvent::getType()
  {
    return TaskType::Invent;
  }
  //------------------------------- GET MINERAL -------------------------------
  BW::TechType TaskInvent::getTechType()
  {
    return this->techType;
  }
  //---------------------------------------------------------------------------
}