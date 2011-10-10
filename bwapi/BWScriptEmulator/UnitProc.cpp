#include "UnitProc.h"
#include "BWScriptEmulator.h"
#include "UType.h"
#include "OrderID.h"
#include "Starcraft.h"
#include "UnitID.h"

UnitProc *getUnitProc(BWAPI::Unit *pUnit)
{
  if ( pUnit->getPlayer() != self )
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

UnitProc::UnitProc()
{ this->Init(); }

UnitProc::UnitProc(Unit *pUnit)
{ this->Init(pUnit); }

void UnitProc::Init()
{
  this->thisUnit        = NULL;
  this->guardLocation   = Positions::None;
  this->destLocation    = Positions::None;
  this->initialized     = false;
  this->dwState         = 0;
  this->iEmulatedOrder  = Orders::Guard;
}
void UnitProc::Init(Unit *pUnit)
{
  this->thisUnit      = pUnit;
  this->guardLocation = pUnit ? pUnit->getPosition() : Positions::None;
  this->destLocation  = Positions::None;
  this->initialized   = false;
  this->dwState       = 0;
}

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
    if ( !thisUnit->isIdle() )
    {
      BWAPI::Region *r = thisUnit->getRegion();
      std::vector<BWAPI::Region*> movetoRgnList;
      for ( std::set<BWAPI::Region*>::const_iterator i = r->getNeighbors().begin(); i != r->getNeighbors().end(); ++i )
      {
        std::set<BWAPI::Unit*> units = BWAPI::Broodwar->getUnitsInRectangle((*i)->getBoundsLeft(), (*i)->getBoundsTop(), (*i)->getBoundsRight(), (*i)->getBoundsBottom() );
        for ( std::set<BWAPI::Unit*>::const_iterator u = units.begin(); u != units.end(); ++u )
        {
          if ( (*u)->getPlayer() != self )
            continue;

          movetoRgnList.push_back(*i);
          break;
        }
      }
      if ( !movetoRgnList.empty() )
        thisUnit->move(movetoRgnList[Random(0, movetoRgnList.size())]->getCenter());
    }
    break;
  default:
    break;
  }
}

void UnitProc::execute()
{
  
}
