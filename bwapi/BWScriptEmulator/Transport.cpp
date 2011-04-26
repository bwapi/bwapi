#include "Transport.h"
#include "BWScriptEmulator.h"

Transport::Transport()
{
  this->Init();
}

Transport::Transport(Unit *pUnit)
{
  this->Init(pUnit);
}

Transport::~Transport()
{}

bool Transport::GenericIdle()
{
  Position p = this->thisUnit->getPosition();
  UnitType thisType = thisUnit->getType();
  if ( thisUnit->isIdle() && thisUnit->getLoadedUnits().size() )
    return thisUnit->unloadAll(p);

  int loadQueueCount = 0;
  Position chosen = Positions::None;
  for each ( Unit *u in bw->getUnitsInRectangle(p.x() - 2048, p.y() - 2048, p.x() + 2048, p.y() + 2048) )
  {
    if ( !u || !u->exists() || u->getPlayer() != self || !u->isCompleted() || loadQueueCount >= thisType.spaceProvided() )
      continue;
    UnitType ut = u->getType();
    if ( ut.spaceRequired() > thisType.spaceProvided() - loadQueueCount || u->hasPath(u->getTargetPosition()) )
      continue;

    if ( chosen == Positions::None )
      chosen = u->getTargetPosition();
    else if ( chosen.getApproxDistance(u->getTargetPosition()) > 256 )
      continue;

    if ( thisUnit->load(u, true) )
      loadQueueCount += ut.spaceRequired();
    if ( thisType.spaceProvided() - loadQueueCount == 0 )
      break;
  }
  if ( chosen != Positions::None )
    return thisUnit->move(chosen, true);
  return false;
}
