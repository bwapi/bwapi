#include "Worker.h"
#include "BWScriptEmulator.h"

Worker::Worker()
{
  this->Init();
}

Worker::Worker(Unit *pUnit)
{
  this->Init(pUnit);
}

Worker::~Worker()
{}

bool Worker::GenericIdle()
{
  if ( thisUnit->isCarryingGas() || thisUnit->isCarryingMinerals() )
    return thisUnit->returnCargo();
  Position p = this->thisUnit->getPosition();
  for each ( Unit *u in thisUnit->getUnitsInRadius(512) )
  {
    if ( !u || !u->exists() || u->getPlayer() != self || !u->isCompleted() )
      continue;
    UnitType ut = u->getType();
    if ( !ut.isResourceDepot() )
      continue;

    for each ( Unit *r in u->getUnitsInRadius(256) )
    {
      if ( !r || !r->exists() || !r->isCompleted() )
        continue;
      if ( (r->getType().isMineralField() || (r->getType().isRefinery() && r->getPlayer() == self)) &&
           (u->hasPath(r) || self->completedUnitCount(UnitTypes::Terran_Dropship) || self->completedUnitCount(UnitTypes::Protoss_Shuttle)) &&
           !r->isBeingGathered() )
        return thisUnit->gather(r);
    }
  }
  return false;
}
