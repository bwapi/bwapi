#include "Expansion.h"

#include <Logger.h>

#include "AI.h"
#include "Unit.h"
#include "Globals.h"

namespace BWAI
{
  //------------------------------------ CONSTRUCTOR --------------------------
  Expansion::Expansion(BWAI::Unit* gatherCenter)
   :gatherCenter(gatherCenter)
   ,asignedWorkers(0)
  {
    ai->log->log("New expansion registration started", LogLevel::Detailed);
    
    for (Unit* i = ai->getFirst(); i != NULL; i = i->getNext())
    {
      if (i->isMineral() &&
          i->expansionAssingment == NULL &&
          i->getDistance(this->gatherCenter) < 350)
        this->minerals.push_back(new Mineral(i, this));
    }
    ai->log->log("%d minerals assigned to %s", this->minerals.size(), this->gatherCenter->getName().c_str(), LogLevel::Normal);
    if (this->gatherCenter->lastTrainedUnit == BW::UnitID::None)
      switch (this->gatherCenter->getType().getID())
      {
        case BW::UnitID::Terran_CommandCenter : gatherCenter->lastTrainedUnit = BW::UnitID::Terran_SCV; break;
        case BW::UnitID::Protoss_Nexus        : gatherCenter->lastTrainedUnit = BW::UnitID::Protoss_Probe; break;
        case BW::UnitID::Zerg_Hatchery        : gatherCenter->lastTrainedUnit = BW::UnitID::Zerg_Drone; break;
      }
    gatherCenter->expansionAssingment = this;


  }
  //------------------------------ REMOVE WORKER ------------------------------
  Expansion::~Expansion(void)
  {
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      delete this->minerals[i];
    this->minerals.clear();
  }
  //---------------------------------------------------------------------------
  void Expansion::removeWorker(Unit* worker)
  {
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      if (this->minerals[i]->removeGatherer(worker))
        return;
  }
  //------------------------------ CHECK ASSIGNED WORKERS ---------------------
  bool Expansion::checkAssignedWorkers()
  {   
    bool reselected = false;
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      reselected |= this->minerals[i]->checkAssignedWorkers();
    return reselected;
  }
  //---------------------------------------------------------------------------
  void Expansion::removeMineral(BWAI::Unit* mineral)
  {
    ai->log->log("Mineral will be removed (was gathered out) %s", mineral->getName().c_str(), LogLevel::Important);
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      if (this->minerals[i]->mineral == mineral)
      {
         this->minerals[i];this->minerals.erase(this->minerals.begin() + i);
         break;
      }
      for (std::list<Mineral*>::iterator i = ai->activeMinerals.begin(); i != ai->activeMinerals.end(); ++i)
        if ((*i)->mineral == mineral)
        {
           Mineral* toRemove = *i;
           ai->activeMinerals.erase(i);
           delete toRemove;  // also frees workers
           break;
        }
  }
  //---------------------------------------------------------------------------
}