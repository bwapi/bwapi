#include "CommandInvent.h"

#include <Util/Logger.h>

#include "UnitImpl.h"
#include "GameImpl.h"
#include "PlayerImpl.h"
#include <BW/Unit.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  CommandInvent::CommandInvent(UnitImpl* building, BW::TechType tech)
      : Command(building)
      , tech(tech)
  {
    startFrame = Broodwar->getFrameCount();
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  CommandInvent::~CommandInvent()
  {
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void CommandInvent::execute()
  {
    if (!executors[0]->_exists) return;
    executors[0]->self->order = BW::OrderID::ResearchTech;
    executors[0]->self->tech = tech.getID();
    executors[0]->self->isIdle = false;
    executors[0]->self->remainingResearchTime = tech.getResearchTime();
    PlayerImpl* p = static_cast<PlayerImpl*>(executors[0]->getPlayer());
    if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      p->spend(tech.getMineralPrice(),tech.getGasPrice());
  }
  //------------------------------------------------ GET TYPE ------------------------------------------------
  int CommandInvent::getType()
  {
    return CommandTypes::Invent;
  }
  //----------------------------------------------------------------------------------------------------------
};
