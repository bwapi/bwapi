#include "UnitInfo.h"
#include "Controller.h"

void UnitWrap::RunComputerAI()
{
  // Run this order on a timer
  if ( GetOrderTimer() > 0 )
    return;
  SetOrderTimer(15);

  // Perform generic unit behaviour
  if ( DoUnitBehaviour() )
    return;

  // Run AIController type-specific stuff
  PerformSpecializedUnitControl();

  // Localize unit type
  UnitType t = pUnit->getType();

  // Check if this (building) needs to be repaired
  if ( (GetRace == Races::Terran && IsMechanical && HP_Percent != 100 && IsBuilding && !IsPlagued && !IsIrradiated && !IsWorker)(pUnit)
        && !IsMilitaryCaptain() )
  {
    // Do repair check/assign here
  } 
  else if ( t == UnitTypes::Terran_Siege_Tank_Tank_Mode )    // Siege Tank special
  {
    if ( !MainController.IsCampaign() && pUnit->isIdle() )
      pUnit->siege();
  }
  else if ( t.isBurrowable() )   // Burrowable unit control
  {
    if ( !pUnit->isBurrowed() && t != UnitTypes::Zerg_Drone && pUnit->isIdle() )
      pUnit->burrow();
  }
  else if ( t == UnitTypes::Terran_Medic )   // Medic special
  {
    if ( !HasNoCaptain() )
      SetVirtualUnitOrder(Orders::Enum::Medic);
    //else
    //  Do independant AI Transport stuff
  }
}

void UnitWrap::DecloakCheck()
{
  // Ignore if not cloaked/cloakable
  if ( !pUnit->isCloaked() || !pUnit->getType().isCloakable() )
    return;

  // Ignore if busy with enemy
  if ( HasEnemyTarget() )
    return;

  // Ignore if auto-targetting something?
  //if ( pUnit->AIOrderTargetUnit ) 
  //  return;

  // Ignore if there's a nearby enemy unit
  if ( pUnit->getClosestUnit(IsEnemy && CanAttack, 256) )
    return;

  if ( pUnit->getOrder() != Orders::NukeWait &&
       pUnit->getOrder() != Orders::NukePaint &&
       pUnit->getOrder() != Orders::NukeTrack )
    pUnit->decloak();
}

bool UnitWrap::DoUnitBehaviour()
{
  DecloakCheck();
  
  UnitType t = pUnit->getType();
  switch ( t )
  {
  case UnitTypes::Enum::Protoss_Carrier:
  case UnitTypes::Enum::Protoss_Reaver:
    // @TODO: Check AI reserves
    pUnit->train();
    return false;
  case UnitTypes::Enum::Terran_Dropship:
    if ( pUnit->getHitPoints() < t.maxHitPoints() ) //&& AssignRepairSCV(pUnit) )
    {
      Unit pSCV = nullptr;//AssignRepairSCV(pUnit);
      if ( pSCV )   // do unload, then follow SCV
      {
        if ( pUnit->getLoadedUnits().empty() )
          return pUnit->follow(pSCV);
        else
          return pUnit->unloadAll();
      }
    }
    // do what protoss shuttle does after:
  case UnitTypes::Enum::Protoss_Shuttle:
    //if ( AI_TranpoerAction(pUnit) )
    //  return true;
    if ( pUnit->getLoadedUnits().empty() )
      return false;
    return pUnit->unloadAll();
  case UnitTypes::Enum::Zerg_Overlord:
    if ( pUnit->getLoadedUnits().empty() )
      return false;
    return pUnit->unloadAll();
  case UnitTypes::Enum::Terran_Vulture:
    return false;//pUnit->useTech(TechTypes::Spider_Mines, GetSpiderMineLocation(pUnit));
  case UnitTypes::Enum::Terran_Siege_Tank_Tank_Mode:
    if ( this->GetControlType() == ControlTypes::Guard && pUnit->isIdle() ) // && !AI_Guard_AttackTarget_Proc(unit, 0)
      return pUnit->siege();
    return false;
  case UnitTypes::Enum::Protoss_Shield_Battery:
    if ( pUnit->getEnergy() >= 50 )
    {
      Unitset myUnits(Broodwar->self()->getUnits());
      for ( auto i = myUnits.begin(); i != myUnits.end(); ++i )
      {
        
        // @TODO
        // if ( i can recharge, not attacking, shields > 40 )
        //  recharge the unit
      }
    }
    return false;
  case UnitTypes::Enum::Protoss_Arbiter:
    // Should be "AIOrderTargetUnit" or autotarget
    Unit pBest = pUnit->getClosestUnit(GetPlayer == Broodwar->self() && !IsBuilding && OrderTarget != nullptr, 640);
    if ( pBest != nullptr )
      return pUnit->attack(pBest->getPosition());
    return false;
  }
  return false;
}

