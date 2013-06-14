#include "UnitInfo.h"

using namespace BWAPI;

void UnitWrap::PerformSpecializedUnitControl()
{
  // safety
  if ( !pUnit ) return;

  switch ( GetControlType() )
  {
  case ControlTypes::Worker:
    RunWorkerController();
    break;
  case ControlTypes::Task:
    SetOrderTimer( RunTaskController() ? 45 : 3 );
    break;
  case ControlTypes::Captain:
    RunMilitaryController();
    SetOrderTimer( 3 );
    break;
  default:
    AssignComputerIdleOrder();
    break;
  }
}

bool UnitWrap::IsMilitaryCaptain() const
{
  return GetControlType() == ControlTypes::Captain && 
          (GetCaptainType() == 1 || GetCaptainType() == 2 || GetCaptainType() == 8 || GetCaptainType() == 9 );
}

bool UnitWrap::HasNoCaptain() const
{
  return GetControlType() != ControlTypes::Captain || GetCaptainType() == CaptainTypes::None;
}

bool UnitWrap::HasEnemyTarget()
{
  // Get the target
  BWAPI::Unit pTarg = pUnit->getOrderTarget();
  if ( !pTarg )
    return false;

  // Ignore if not an enemy
  if ( !Broodwar->self()->isEnemy(pTarg->getPlayer()) )
    return false;

  // Ignore if damaged hallucination
  if ( pTarg->isHallucination() && (HP+Shields)(pTarg) != (MaxHP+MaxShields)(pTarg) )
    return false;
  
  // Ignore if it can be attacked and out of weapon range
  if ( pUnit->canIssueCommand( UnitCommand::attack(pUnit, pTarg)) )
    return pUnit->isInWeaponRange(pTarg);

  // Ignore if it's an unloaded bunker
  return pTarg->getType() == UnitTypes::Terran_Bunker && !pTarg->getLoadedUnits().empty();
}
