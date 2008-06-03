#include "Expansion.h"

#include <Util/Logger.h>

#include "AI.h"
#include "Unit.h"
#include "Globals.h"
#include "TaskGather.h"

namespace BWAI
{
  int Expansion::maximumMineralDistance = 350;
  //------------------------------------ CONSTRUCTOR --------------------------
  Expansion::Expansion(BWAI::Unit* gatherCenter)
   :gatherCenter(gatherCenter)
  {
    ai->log->log("New expansion registration started", Util::LogLevel::Detailed);
    
    for (Unit* i = ai->getFirst(); i != NULL; i = i->getNext())
    {
      if (i->isMineral() &&
          i->expansion == NULL &&
          i->getDistance(this->gatherCenter) < maximumMineralDistance)
      {
        i->expansion = this;
        TaskGather* newMineral = new TaskGather(i, this);
        this->minerals.push_back( newMineral);
        BWAI::ai->activeMinerals.push_back(newMineral);
      }
    }
    ai->log->log("%d minerals assigned to %s", this->minerals.size(), this->gatherCenter->getName().c_str(), Util::LogLevel::Normal);
    if (this->gatherCenter->lastTrainedUnit == BW::UnitID::None)
      switch (this->gatherCenter->getType().getID())
      {
        case BW::UnitID::Terran_CommandCenter : gatherCenter->lastTrainedUnit = BW::UnitID::Terran_SCV; break;
        case BW::UnitID::Protoss_Nexus        : gatherCenter->lastTrainedUnit = BW::UnitID::Protoss_Probe; break;
        case BW::UnitID::Zerg_Hatchery        : gatherCenter->lastTrainedUnit = BW::UnitID::Zerg_Drone; break;
      }
    gatherCenter->expansion = this;
  }
  //--------------------------------- DESTRUCTOR ------------------------------
  Expansion::~Expansion(void)
  {
    while (!this->minerals.empty())
      this->removeMineral(this->minerals.front()->getMineral());
    if (this->gatherCenter != NULL &&
        this->gatherCenter->expansion == this)
      this->gatherCenter->expansion = NULL;
  }
  //---------------------------------------------------------------------------
  void Expansion::removeMineral(BWAI::Unit* mineral)
  {
    ai->log->log("Mineral will be removed (was gathered out) %s", mineral->getName().c_str(), Util::LogLevel::Important);
    
    for (std::list<TaskGather*>::iterator i = this->minerals.begin(); i != this->minerals.end(); ++i)
      if ((*i)->getMineral() == mineral)
      {
        ai->log->log("Mineral removed from Expansion");
        this->minerals.erase(i);
        break;
      }
    for (std::list<TaskGather*>::iterator i = ai->activeMinerals.begin(); i != ai->activeMinerals.end(); ++i)
      if ((*i)->getMineral() == mineral)
        {
          ai->log->log("Mineral removed from activeMinerals");
          delete *i;
          ai->activeMinerals.erase(i);
          return;
        }
  }
  //---------------------------------------------------------------------------
}