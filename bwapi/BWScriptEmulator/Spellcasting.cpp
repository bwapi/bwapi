#include "UnitProc.h"
#include "Spellcasting.h"
#include "BWScriptEmulator.h"
#include <vector>

std::vector<spell> spellsCast;

bool UnitProc::AbilityInUse(TechType tech, Unit *target)
{
  // Find out if we've already assigned a unit for this tech casting task
  for ( std::vector<spell>::iterator i = spellsCast.begin(); i != spellsCast.end(); ++i )
    if ( i->pUnit == target && i->tech == tech )
      return true;
  return false;
}

bool UnitProc::UseAbility(TechType tech, Unit *target)
{
  // Use the tech
  if ( this->thisUnit->useTech(tech, target) )
  {
    spell spl = { target, tech };
    spellsCast.push_back(spl);
    return true;
  }
  return false;
}

bool UnitProc::CheckForSpellcasting()
{
  Order o = this->thisUnit->getOrder();
  Unit *t = this->thisUnit->getOrderTarget();
  if ( t )
  {
    if (  ( o == Orders::CastDefensiveMatrix  && t->getDefenseMatrixPoints() > 20 )       ||
          ( o == Orders::CastEMPShockwave     && t->getShields() + t->getEnergy() < 40 )  ||
          ( o == Orders::CastEnsnare          && t->getEnsnareTimer() > 5 )               ||
          ( o == Orders::CastFeedback         && t->getEnergy() < 30 )                    ||
          ( o == Orders::CastIrradiate        && t->getIrradiateTimer() > 5 )             ||
          ( o == Orders::CastLockdown         && t->getLockdownTimer() > 5 )              ||
          ( o == Orders::CastMaelstrom        && t->getMaelstromTimer() > 5 )             ||
          ( o == Orders::CastMindControl      && t->getPlayer() == self )                 ||
          ( o == Orders::CastOpticalFlare     && t->isBlind() )                           ||
          ( o == Orders::CastParasite         && t->isParasited() )                       ||
          ( o == Orders::CastPlague           && t->getPlagueTimer() > 5 )                ||
          ( o == Orders::CastPsionicStorm     && t->isUnderStorm() )                      ||
          ( o == Orders::CastRestoration      && !t->isParasited()
                                              && !t->isBlind()
                                              && !t->isEnsnared()
                                              && !t->isIrradiated()
                                              && !t->isMaelstrommed()
                                              && !t->isPlagued()
                                              && !t->getAcidSporeCount() )                ||
          ( o == Orders::CastStasisField      && !t->isStasised() )                       ||
          ( o == Orders::CastDisruptionWeb    && t->isUnderDisruptionWeb() )              ||
          ( o == Orders::CastDarkSwarm        && t->isUnderDarkSwarm() )                  
       )
      return thisUnit->stop();
  }
  return false;
}
