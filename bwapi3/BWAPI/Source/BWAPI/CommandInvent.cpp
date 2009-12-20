#include "CommandInvent.h"

#include <Util/Logger.h>

#include "UnitImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandInvent::CommandInvent(UnitImpl* building, BW::TechType tech)
      : Command(building)
      , tech(tech)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandInvent::~CommandInvent()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandInvent::execute()
  {
    if (!this->executors[0]->_exists()) return;
    if (this->executors[0]->isCompleted())
    {
      this->executors.front()->getRawDataLocal()->orderID = BW::OrderID::ResearchTech;
      this->executors.front()->getRawDataLocal()->childUnitUnion2.unitIsNotScarabInterceptor.subChildUnitUnion1.techID = tech.getID();
    }
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandInvent::getType()
  {
    return CommandTypes::Invent;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandInvent::describe()
  {
    return this->executors[0]->getName() + " will invent (" + tech.getName() + ")";
  }
  //----------------------------------------------------------------------------------------------------------
};
