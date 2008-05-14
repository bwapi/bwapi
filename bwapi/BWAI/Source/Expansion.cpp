#include "Expansion.h"
#include "AI.h"
#include "Unit.h"
#include "Globals.h"
#include "..//..//BWAPI//Source//BWAPI//UnitPrototype.h" /**< @todo remove */
namespace BWAI
{
  //------------------------------------ CONSTRUCTOR --------------------------
  Expansion::Expansion(BWAI::Unit* gatherCenter)
   :gatherCenter(gatherCenter)
   ,asignedWorkers(0)
  {
    ai->log->log("New expansion registration started", Logger::Detailed);
    
    for (Unit* i = ai->getFirst(); i != NULL; i = i->getNext())
    {
      if (i->isMineral() &&
          i->expansionAssingment == NULL &&
          i->getDistance(this->gatherCenter) < 350)
        this->minerals.push_back(new Mineral(i, this));
    }
    ai->log->log("%d minerals assigned to %s", this->minerals.size(), this->gatherCenter->getName().c_str(), Logger::Normal);
    if (this->gatherCenter->lastTrainedUnitID == BW::UnitType::None)
      switch (this->gatherCenter->getType())
      {
        case BW::UnitType::Terran_CommandCenter : gatherCenter->lastTrainedUnitID = BW::UnitType::Terran_SCV; break;
        case BW::UnitType::Protoss_Nexus        : gatherCenter->lastTrainedUnitID = BW::UnitType::Protoss_Probe; break;
        case BW::UnitType::Zerg_Hatchery        : gatherCenter->lastTrainedUnitID = BW::UnitType::Zerg_Drone; break;
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
    for (unsigned int i = 0; i < this->minerals.size(); i++)
      if (this->minerals[i]->mineral == mineral)
      {
         delete this->minerals[i]; // also frees workers
         this->minerals.erase(this->minerals.begin() + i);
         return;
      }
  }
  //---------------------------------------------------------------------------
}