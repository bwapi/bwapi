#include "UnitProc.h"
#include "BWScriptEmulator.h"
#include "Spellcasting.h"

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

void UnitProc::OnIdle()
{
  // Check existence and frame time
  if ( !this->thisUnit || !this->thisUnit->exists() || this->thisUnit->getLastCommandFrame() + bw->getLatencyFrames() > bw->getFrameCount() )
    return;
  // Check if unit is completed
  if ( !initialized )
  {
    if ( !this->thisUnit->isCompleted() )
      return;
    initialized   = true;
    guardLocation = this->thisUnit->getPosition();
  }
  // Heal self check
  if ( this->HealSelf() )
    return;
  // Prevent multi-spell casting
  if ( this->CheckForSpellcasting() )
    return;
  if ( thisUnit->isIdle() && thisUnit->getLastCommandFrame() < bw->getFrameCount() + 20 )
  {
    // Idle check
    if ( this->GenericIdle() )
      return;
  }
}

bool UnitProc::HealSelf()
{
  return false;
}

bool UnitProc::CanUse(TechType tech)
{
  UnitType thisType = thisUnit->getType();
  if ( !thisType.isHero() && !self->hasResearched(tech) )
    return false;
  if ( thisUnit->getEnergy() < tech.energyUsed() )
    return false;
  for each ( TechType t in thisType.abilities() )
  {
    if ( t == tech )
      return true;
  }
  return false;
}

bool UnitProc::WorthHitting(Unit *target)
{
  if ( !target || !target->exists() || target->isInvincible() || target->isHallucination() )
    return false;
  if ( self->isAlly(target->getPlayer()) || target->getPlayer() == self )
    return false;
  
  UnitType targType = target->getType();
  if ( targType == UnitTypes::Protoss_Interceptor         ||
       targType == UnitTypes::Protoss_Scarab              ||
       targType == UnitTypes::Terran_Vulture_Spider_Mine  ||
       target->isGatheringGas() )
    return false;
  return true;
}

bool UnitProc::CanHit(WeaponType wpn, Unit *target)
{
  if ( wpn >= WeaponTypes::None || !target || !target->exists() || target->isInvincible() )
    return false;

  UnitType targType = target->getType();
  if ( ( !wpn.targetsAir()        && (target->isLifted()  || targType.isFlyer())        ) ||
       ( !wpn.targetsGround()     && (!target->isLifted() && !targType.isFlyer())       ) ||
       ( wpn.targetsMechanical()  && !targType.isMechanical()                           ) ||
       ( wpn.targetsOrganic()     && !targType.isOrganic()                              ) ||
       ( wpn.targetsNonBuilding() && targType.isBuilding()                              ) ||
       ( wpn.targetsNonRobotic()  && targType.isRobotic()                               ) ||
       ( wpn.targetsOrgOrMech()   && !targType.isOrganic() && !targType.isMechanical()  ) ||
       ( wpn.targetsOwn()         && target->getPlayer() != self                        ) )
    return false;
  return true;
}

// Virtual functions
bool UnitProc::GenericIdle()
{
  return false;
}
