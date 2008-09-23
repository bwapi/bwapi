#include "Expansion.h"

#include <Util/Logger.h>

#include "AI.h"
#include "Unit.h"
#include "Globals.h"
#include "TaskGather.h"

namespace BWAI
{
  int Expansion::maximumMineralDistance = 350;
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Expansion::Expansion(BWAI::Unit* gatherCenter)
   :gatherCenter(gatherCenter)
  {
    ai->log->logDetailed("New expansion registration started");
    
    for each (Unit* i in ai->units)
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
    ai->log->logCommon("%d mineral fields assigned to %s", this->minerals.size(), this->gatherCenter->getName().c_str());
    gatherCenter->expansion = this;
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Expansion::~Expansion(void)
  {
    while (!this->minerals.empty())
      this->removeMineral(this->minerals.front()->getMineral());
    if (this->gatherCenter != NULL &&
        this->gatherCenter->expansion == this)
      this->gatherCenter->expansion = NULL;
  }
  //----------------------------------------------------------------------------------------------------------
  void Expansion::removeMineral(BWAI::Unit* mineral)
  {
    ai->log->logCommon("Mineral will be removed (was gathered out or end game) %s", mineral->getName().c_str());
    mineral->expansion = NULL;
    
    for (std::list<TaskGather*>::iterator i = this->minerals.begin(); i != this->minerals.end(); ++i)
      if ((*i)->getMineral() == mineral)
      {
        this->minerals.erase(i);
        break;
      }
    for (std::list<TaskGather*>::iterator i = ai->activeMinerals.begin(); i != ai->activeMinerals.end(); ++i)
      if ((*i)->getMineral() == mineral)
        {
          delete *i;
          ai->activeMinerals.erase(i);
          return;
        }
  }
  //----------------------------------------------------------------------------------------------------------
}