#include "UnitProc.h"
#include "BWScriptEmulator.h"
#include "UType.h"
#include "OrderID.h"
#include "UnitID.h"

UnitProc *getUnitProc(BWAPI::Unit *pUnit)
{
  if ( !pUnit || pUnit->getPlayer() != self )
    return NULL;

  switch ( pUnit->getType() )
  {
  case UnitID::Terran_ScienceVessel:
    break;
  default:
    break;
  }
  return new UnitProc(pUnit);
}

UnitProc::UnitProc(Unit *pUnit)
: thisUnit(pUnit)
, guardLocation(pUnit ? pUnit->getPosition() : Positions::None)
, destLocation(Positions::None)
, initialized(false)
, dwState(0)
, iEmulatedOrder(pUnit ? UType(pUnit->getType()).aiIdle() : Orders::Guard)
, aiCaptain(0)
{ }

UnitProc::~UnitProc()
{}

void UnitProc::EmulateIdleOrder()
{
  UType t = thisUnit->getType();
  if ( thisUnit->isIdle() )
    this->iEmulatedOrder = t.aiIdle();
  
  switch ( this->iEmulatedOrder )
  {
  case OrderID::AIPatrol:
    if ( thisUnit->isIdle() )
    {
      // Get the region
      BWAPI::Region *r = thisUnit->getRegion();
      std::vector<BWAPI::Region*> movetoRgnList;

      // iterate all regions neighboring this unit
      for ( std::set<BWAPI::Region*>::const_iterator i = r->getNeighbors().begin(); i != r->getNeighbors().end(); ++i )
      {
        // iterate all our units
        for ( std::set<BWAPI::Unit*>::const_iterator u = self->getUnits().begin(), 
              uend = self->getUnits().end(); 
              u != uend; 
              ++u )
        {
          UnitType ut = (*u)->getType();
          if ( !ut.isBuilding() || !(*u)->exists() )  // Ignore non-buildings
            continue;

          // Save the region if its center is within sight range of the building
          if ( (*u)->getDistance( (*i)->getCenter() ) <= ut.sightRange() )
          {
            movetoRgnList.push_back(*i);
            break;
          }
        }// iterate self units
      }// iterate regions
      if ( !movetoRgnList.empty() )
        thisUnit->move(movetoRgnList[rand() % movetoRgnList.size()]->getCenter());
    }
    break;
  default:
    break;
  }
}

void UnitProc::StandardUnitProc()
{
  this->EmulateIdleOrder();
}

void UnitProc::execute()
{
  switch ( this->aiCaptain )
  {
  case 0:
    this->StandardUnitProc();
    break;
  }
}
