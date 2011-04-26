#include "ScienceVessel.h"
#include "BWScriptEmulator.h"
#include "Spellcasting.h"

ScienceVessel::ScienceVessel()
{
  this->Init();
}

ScienceVessel::ScienceVessel(Unit *pUnit)
{
  this->Init(pUnit);
}

ScienceVessel::~ScienceVessel()
{}

bool ScienceVessel::GenericIdle()
{
  Position p = this->thisUnit->getPosition();
  int castRadius = (farcasting ? 2048 : 288);
  if ( this->CanUse(TechTypes::EMP_Shockwave) )
  {
    Unit *bestTarget = NULL;
    for each ( Unit *u in bw->getUnitsInRectangle(p.x() - castRadius, p.y() - castRadius, p.x() + castRadius, p.y() + castRadius) )
    {
      if ( !this->WorthHitting(u) || !this->CanHit(WeaponTypes::EMP_Shockwave, u) )
        continue;

      if ( !u->getType().isSpellcaster() && !u->getType().maxShields() )
        continue;

      unsigned int searchValue = 0;
      Position ep = u->getPosition();
      for each ( Unit *clump in bw->getUnitsInRectangle(ep.x() - 160, ep.y() - 160, ep.x() + 160, ep.y() + 160) )
      {
        if ( !this->WorthHitting(clump) )
          continue;
        if ( clump->getType().maxShields() )
          searchValue += clump->getShields();
        if ( clump->getType().isSpellcaster() && !clump->isHallucination() )
          searchValue += clump->getEnergy();
      }
      if ( searchValue >= 200 && this->AbilityInUse(TechTypes::EMP_Shockwave, u) )
      {
        bestTarget = u;
        break;
      }
    }
    if ( bestTarget && this->UseAbility(TechTypes::EMP_Shockwave, bestTarget) )
      return true;
  }
  if ( this->CanUse(TechTypes::Defensive_Matrix) )
  {
    Unit *bestTarget = NULL;
    for each ( Unit *u in bw->getUnitsInRectangle(p.x() - castRadius, p.y() - castRadius, p.x() + castRadius, p.y() + castRadius) )
    {
      if ( !u || !u->exists()                 ||
           u->getDefenseMatrixPoints()        || 
           self->isEnemy(u->getPlayer())      || 
           u->isCloaked()                     || 
           u->isBurrowed()                    ||
           !u->getOrderTarget()               ||
           u->getType().maxHitPoints() < 100  ||
           u->getHitPoints() > 75             ||
           u->isInvincible()                  ||
           u->getType().isBuilding()          ||
           this->AbilityInUse(TechTypes::Defensive_Matrix, u) )
        continue;
      bestTarget = u;
      break;
    }
    if ( bestTarget && this->UseAbility(TechTypes::Defensive_Matrix, bestTarget) )
      return true;
  }
  if ( this->CanUse(TechTypes::Irradiate) )
  {
    Unit *bestTarget = NULL;
    for each ( Unit *u in bw->getUnitsInRectangle(p.x() - castRadius, p.y() - castRadius, p.x() + castRadius, p.y() + castRadius) )
    {
      if ( !this->WorthHitting(u)     || 
           u->getIrradiateTimer()     ||
           !this->CanHit(WeaponTypes::Irradiate, u) )
        continue;
      UnitType ut = u->getType();
      if ( !ut.isOrganic() ||
           ut.isBuilding() ||
           ut == UnitTypes::Zerg_Larva ||
           ut == UnitTypes::Zerg_Egg   ||
           ut == UnitTypes::Zerg_Lurker_Egg ||
           this->AbilityInUse(TechTypes::Irradiate, u))
        continue;
      bestTarget = u;
      break;
    }
    if ( bestTarget && this->UseAbility(TechTypes::Irradiate, bestTarget) )
      return true;
  }
  return false;
}
