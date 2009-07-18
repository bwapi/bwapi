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
    if (this->executors[0]->isReady())
      this->executors.front()->getRawDataLocal()->orderID = BW::OrderID::ResearchTech;
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  BWAPI::CommandTypes::Enum CommandInvent::getType()
  {
    return CommandTypes::Invent;
  }
  //----------------------------------------------------------------------------------------------------------
  std::string CommandInvent::describe()
  {
    return this->executors[0]->getName() + " will invent (" + tech.getName() + ")";
  }
  //----------------------------------------------------------------------------------------------------------
}