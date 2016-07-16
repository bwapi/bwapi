#include "OrderEmulate.h"
#include "ScriptEnum.h"
#include "UnitInfo.h"

void UnitWrap::UpdateOrderTimers()
{
  int timer;

  // Order timer
  timer = GetOrderTimer();
  if ( timer > 0 )
    SetOrderTimer(timer-1);

}

void UnitWrap::EmulateOrder()
{
  // Update all countdown timers (order timer is an important update)
  UpdateOrderTimers();

  // Safety check so that we don't interrupt orders (for some reason the AI enjoys spam)
  if ( Broodwar->getFrameCount() <= (pUnit->getLastCommandFrame() + Broodwar->getLatencyFrames()) )
    return;

  // Switch for emulated order IDs
  switch ( GetUnitOrder() )
  {
  case Orders::Enum::JunkYardDog:
    RunJunkYardDog();
    break;
  case Orders::Enum::ComputerAI:
    RunComputerAI();
    break;
  case Orders::Enum::Guard:
    RunGuard();
    break;
  case Orders::Enum::GuardPost:
    RunGuardPost();
    break;
  case Orders::Enum::ComputerReturn:
    RunComputerReturn();
    break;
  case Orders::Enum::CastDarkSwarm:
  case Orders::Enum::CastDefensiveMatrix:
  case Orders::Enum::CastDisruptionWeb:
  case Orders::Enum::CastEMPShockwave:
  case Orders::Enum::CastEnsnare:
  case Orders::Enum::CastFeedback:
  case Orders::Enum::CastHallucination:
  case Orders::Enum::CastInfestation:
  case Orders::Enum::CastIrradiate:
  case Orders::Enum::CastLockdown:
  case Orders::Enum::CastMaelstrom:
  case Orders::Enum::CastMindControl:
  case Orders::Enum::CastNuclearStrike:
  case Orders::Enum::CastOpticalFlare:
  case Orders::Enum::CastParasite:
  case Orders::Enum::CastPlague:
  case Orders::Enum::CastPsionicStorm:
  case Orders::Enum::CastRecall:
  case Orders::Enum::CastRestoration:
  case Orders::Enum::CastScannerSweep:
  case Orders::Enum::CastSpawnBroodlings:
  case Orders::Enum::CastStasisField:
  case Orders::Enum::FireYamatoGun:
    if ( pUnit->isIdle() )
      AssignComputerIdleOrder();
    break;
  }
}

BWAPI::Unit UnitWrap::FindNewAttackTarget() const
{
  UnitType t = pUnit->getType();
  
  Unit pTarg = pUnit->getClosestUnit( IsEnemy && 
                                      [this](Unit u){ return pUnit->isInWeaponRange(u);} &&
                                      IsVisible &&
                                      !IsInvincible,  std::max(t.seekRange(),t.sightRange())*32 );
  // Should be greatly expanded, but for now something simple
  return pTarg;
}
