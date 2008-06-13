#include "TaskInvent.h"

#include "Globals.h"
#include <Util/Logger.h> //@todo remove

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
           {
            this->addExecutor(i);
            break;
           }
      }
    if (this->executors.empty())
      return false;
    if (!this->executors.front()->isReady() ||
        this->executors.front()->getOrderID() != BW::OrderID::Nothing2)
      return false;
    this->executors.front()->invent(this->techType);
    return true;
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