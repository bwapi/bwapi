#include "UnitInfo.h"
#include "Controller.h"

bool UnitWrap::TaskBunkerDefender() // @TODO
{
  return false;
}

auto TargetWorthHitting = [](Unit pTarg)->bool
{
  UnitType t = pTarg->getType();
  Order o = pTarg->getOrder();
  return pTarg->isVisible() &&
          Broodwar->self()->isEnemy(pTarg->getPlayer()) &&
          t != UnitTypes::Protoss_Interceptor &&
          t != UnitTypes::Terran_Vulture_Spider_Mine &&
          t != UnitTypes::Protoss_Scarab &&
          o != Orders::MoveToGas &&
          o != Orders::WaitForGas &&
          o != Orders::HarvestGas;
};

BWAPI::UnitFilter WeaponCanHit(WeaponType wpn)
{
  return [wpn](Unit u)->bool
  {
    UnitType ut = u->getType();
    return !(( wpn.targetsOwn()         && u->getPlayer() != Broodwar->self() ) ||
            ( !wpn.targetsAir()         && !u->isFlying()       ) ||
            ( !wpn.targetsGround()      && u->isFlying()        ) ||
            ( wpn.targetsMechanical()   && ut.isMechanical()    ) ||
            ( wpn.targetsOrganic()      && ut.isOrganic()       ) ||
            ( wpn.targetsNonBuilding()  && !ut.isBuilding()     ) ||
            ( wpn.targetsNonRobotic()   && !ut.isRobotic()      ) ||
            ( wpn.targetsOrgOrMech()    && (ut.isOrganic() || ut.isMechanical()) ));
  };
};

auto LockdownReqs = [](Unit pTarg)->bool
{
  if ( pTarg->isLockedDown() || pTarg->isStasised() || pTarg->isMaelstrommed() || 
      !TargetWorthHitting(pTarg) || !WeaponCanHit(WeaponTypes::Lockdown)(pTarg) )
      return false;
  UnitType t = pTarg->getType();
  if ( (t == UnitTypes::Protoss_Shuttle || t == UnitTypes::Terran_Dropship) && !pTarg->getLoadedUnits().empty() )
    return true;
  if ( t == UnitTypes::Protoss_Carrier || t == UnitTypes::Protoss_Reaver )
    return true;
  Unit pEnemyOfMyEnemey = pTarg->getOrderTarget();
  return pEnemyOfMyEnemey && IsAlly(pEnemyOfMyEnemey);
};

auto DMatrixReqs = [](Unit pTarg)->bool
{
  return !( pTarg->isDefenseMatrixed() || !IsAlly(pTarg) || !pTarg->getOrderTarget() || pTarg->isCloaked() ||
            MaxHP(pTarg) < 100 || pTarg->getHitPoints() > 75 || pTarg->isInvincible() || IsBuilding(pTarg) );
};

auto EMPShieldReqs = [](Unit pTarg)->bool
{
  if ( !TargetWorthHitting(pTarg) || !MaxShields(pTarg) || !WeaponCanHit(WeaponTypes::EMP_Shockwave)(pTarg) )
    return false;

  int totalShields = 0;
  auto p = pTarg->getPosition();
  Broodwar->getUnitsInRectangle(p.x-160, p.y-160, p.x+160, p.y+160,
                                [&totalShields](Unit u)->bool{ if ( !u->isInvincible() && IsEnemy(u) && MaxShields(u) > 0 )
                                                                  totalShields += u->getShields();
                                                                return false;} );
  return totalShields >= 200;
};

auto EMPEnergyReqs = [](Unit pTarg)->bool
{
  if ( !TargetWorthHitting(pTarg) || !IsSpellcaster(pTarg) || pTarg->isHallucination() || !WeaponCanHit(WeaponTypes::EMP_Shockwave)(pTarg) )
    return false;

  int totalEnergy = 0;
  auto p = pTarg->getPosition();
  Broodwar->getUnitsInRectangle(p.x-160, p.y-160, p.x+160, p.y+160,
                                [&totalEnergy](Unit u)->bool{ if ( !u->isInvincible() && IsEnemy(u) && IsSpellcaster(u) && !u->isHallucination() )
                                                                  totalEnergy += u->getEnergy();
                                                                return false;} );
  return totalEnergy >= 200;
};

auto IrradiateReqs = [](Unit pTarg)->bool
{
  UnitType t = pTarg->getType();
  return TargetWorthHitting(pTarg) && !pTarg->isIrradiated() && WeaponCanHit(WeaponTypes::Irradiate)(pTarg) &&
          t.isOrganic() && !t.isBuilding() && t != UnitTypes::Zerg_Larva && t != UnitTypes::Zerg_Egg && t != UnitTypes::Zerg_Lurker_Egg;
};

auto RestrictedIrradiateReqs = [](Unit pTarg)->bool
{
  UnitType t = pTarg->getType();
  return IrradiateReqs(pTarg) && (!MainController.IsCampaign() || t.isWorker() || t == UnitTypes::Zerg_Overlord || t == UnitTypes::Terran_Medic);
};

auto YamatoReqs = [](Unit pTarg)->bool
{
  if ( !TargetWorthHitting(pTarg) )
    return false;

  UnitType t = pTarg->getType();
  if ( t.isBuilding() && t.canAttack() )
    return true;
  if ( t == UnitTypes::Terran_Bunker && !pTarg->getLoadedUnits().empty() )
    return true;
  if ( t == UnitTypes::Zerg_Egg )
    return false;
  
  int hits = (HP+Shields)(pTarg);
  if ( 200 <= hits && hits <= 450 )
    return !t.isHero();
  return false;
};

auto HallucinationReqs = [](Unit pTarg)->bool
{
  UnitType t = pTarg->getType();
  return pTarg->getPlayer() == Broodwar->self() && !pTarg->isInvincible() && !t.isBuilding() && t != UnitTypes::Protoss_Interceptor &&
          (t == UnitTypes::Protoss_Carrier || t == UnitTypes::Protoss_Scout || t == UnitTypes::Protoss_Arbiter || t == UnitTypes::Protoss_Archon);
};

bool UnitWrap::TaskSpellcaster(bool isAggressive) // @TODO
{
  int range;

  if ( !isAggressive && MainController.GetCastingCooldown() != 0 )
    return false;

  switch ( pUnit->getType() )
  {
    // ### TERRAN
  case UnitTypes::Enum::Terran_Ghost:
    if ( CanUseTech(TechTypes::Lockdown) )
    {
      if ( isAggressive )
        range = 288;
      else
        range = MainController.IsCampaign() ? 2048 : 1024;

      if ( UseTechOnClosest(TechTypes::Lockdown, range, LockdownReqs) )
        return true;
    }
    // @TODO: Nuke script
    break;
  case UnitTypes::Enum::Terran_Science_Vessel:
    range = isAggressive ? 288 : 2048;
    if ( CanUseTech(TechTypes::EMP_Shockwave) )
    {
      // Get best target unit
      Unit pTarget = pUnit->getClosestUnit(EMPShieldReqs, range);
      if ( !pTarget && !isAggressive )
        pTarget = pUnit->getClosestUnit(EMPEnergyReqs, range);

      if ( UseTech(TechTypes::EMP_Shockwave, pTarget) )
        return true;
    }
    if ( CanUseTech(TechTypes::Defensive_Matrix) )
    {
      if ( UseTechOnClosest(TechTypes::Defensive_Matrix, range, DMatrixReqs) )
        return true;
    }
    if ( CanUseTech(TechTypes::Irradiate) )
    {      
      if ( isAggressive )
      {
        if ( UseTechOnClosest(TechTypes::Irradiate, range, IrradiateReqs) )
          return true;
      }
      else
      {
        if ( UseTechOnClosest(TechTypes::Irradiate, range, RestrictedIrradiateReqs) )
          return true;
      }
    }
    break;
  case UnitTypes::Enum::Terran_Battlecruiser:
    if ( CanUseTech(TechTypes::Yamato_Gun) )
    {
      if ( UseTechOnClosest(TechTypes::Yamato_Gun, isAggressive ? 512 : 9999999, YamatoReqs) )
        return true;
    }
    break;
  case UnitTypes::Enum::Terran_Medic:
    if ( CanUseTech(TechTypes::Restoration) )
    {
      // @TODO
    }
    if ( CanUseTech(TechTypes::Optical_Flare) )
    {
      // @TODO
    }
    break;
    // ### ZERG
  case UnitTypes::Enum::Zerg_Defiler:
    if ( CanUseTech(TechTypes::Plague) )
    {
      // @TODO
    }
    if ( CanUseTech(TechTypes::Dark_Swarm) )
    {
      // @TODO
    }
    break;
  case UnitTypes::Enum::Zerg_Queen:
    // @TODO
    break;
    // ### PROTOSS
  case UnitTypes::Enum::Protoss_Arbiter:
    if ( CanUseTech(TechTypes::Stasis_Field) )
    {
      // @TODO
    }
    if ( CanUseTech(TechTypes::Recall) )
    {
      // @TODO
    }
    break;
  case UnitTypes::Enum::Protoss_High_Templar:
    if ( CanUseTech(TechTypes::Psionic_Storm) )
    {
      // @TODO
    }
    if ( CanUseTech(TechTypes::Hallucination) && !isAggressive )
    {
      if ( UseTechOnClosest(TechTypes::Hallucination, 1024, HallucinationReqs) )
        return true;
    }
    break;
  case UnitTypes::Enum::Protoss_Corsair:
    if ( CanUseTech(TechTypes::Disruption_Web) )
    {
      // @TODO
    }
    break;
  case UnitTypes::Enum::Protoss_Dark_Archon:
    if ( CanUseTech(TechTypes::Feedback) )
    {
      // @TODO
    }
    if ( CanUseTech(TechTypes::Mind_Control) )
    {
      // @TODO
    }
    if ( CanUseTech(TechTypes::Maelstrom) )
    {
      // @TODO
    }
    break;
  }
  return false;
}

bool UnitWrap::TaskAggression() // @TODO
{
  return false;
}


void UnitWrap::RunMilitaryController()
{
  // Run bunker tasks
  if ( (pUnit->getType() == UnitTypes::Terran_Marine || pUnit->getType() == UnitTypes::Terran_Firebat) &&
        TaskBunkerDefender() )
    return;

  // Run spellcaster tasks
  if ( TaskSpellcaster(false) )
    return;

  // Run aggressive unit tasks (large)
  if ( TaskAggression() )
    return;

  // @TODO
  Region targetRgn = nullptr;
  // Get targetRgn from captain destination or slowest unit in group
  
  if ( pUnit->getRegion() == targetRgn )
    pUnit->holdPosition();
  else
    pUnit->attack(targetRgn->getCenter());
}
