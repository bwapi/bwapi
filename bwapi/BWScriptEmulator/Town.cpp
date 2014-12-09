#include "Town.h"
#include "UnitInfo.h"

using namespace BWAPI;

Town::Town()
  : desiredWorkerCount(0)
  , currentWorkerCount(0)
  , location(BWAPI::Positions::Origin)
  , pTownCenter(nullptr)
{}

Town::Town( BWAPI::Position townLocation )
  : desiredWorkerCount(0)
  , currentWorkerCount(0)
  , location(townLocation)
  , pTownCenter(nullptr)
{}

void Town::AddUnit(BWAPI::Unit pUnit)
{
  if ( !pUnit )
    return;

  if ( pUnit->getType().isWorker() )
  {
    // New Worker
    UnitWrap(pUnit).SetControlType(ControlTypes::Worker);
  }
  else if ( (IsBuilding || GetType == UnitTypes::Zerg_Egg || GetType == UnitTypes::Zerg_Larva || GetType == UnitTypes::Zerg_Overlord)(pUnit) )
  {
    // New Task

    // Something make guard

    //
    if ( pUnit->getType().isResourceDepot() )
    {
      if ( !this->pTownCenter )
        this->pTownCenter = pUnit;
    }
    else if ( pUnit->getType().isRefinery() )
    {
      this->gasGeysers.insert(pUnit);
    }
  }
}

