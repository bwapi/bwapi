#include <BWAPI/Filters.h>
#include <BWAPI/Unit.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Player.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Game.h>
#include <BWAPI/Order.h>
#include <BWAPI/UnitSizeType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/WeaponType.h>

#define U_UFILTER(n,x) bool n ## Impl(const Unit &u){ return(x); } const PtrUnitFilter n( &n ## Impl )
#define U_CFILTER(n,x) int n ## Impl(const Unit &u){ return (x); } const PtrCompareUnitFilter<int> n( &n ## Impl )
#define U_CFILTERX(n,x,r) r n ## Impl(const Unit &u){ return (x); } const PtrCompareUnitFilter<r> n( &n ## Impl )

namespace BWAPI
{
  namespace Filter
  {
    U_UFILTER(IsTransport, u->getType().spaceProvided() > 0 && u->getType() != UnitTypes::Terran_Bunker);
    U_UFILTER(CanProduce, u->getType().canProduce() );
    U_UFILTER(CanAttack, u->getType().canAttack() );
    U_UFILTER(CanMove, u->getType().canMove() );
    U_UFILTER(IsFlyer, u->getType().isFlyer() );
    U_UFILTER(IsFlying, u->isFlying() );
    U_UFILTER(RegeneratesHP, u->getType().regeneratesHP() );
    U_UFILTER(IsSpellcaster, u->getType().isSpellcaster() );
    U_UFILTER(HasPermanentCloak, u->getType().hasPermanentCloak() );
    U_UFILTER(IsOrganic, u->getType().isOrganic() );
    U_UFILTER(IsMechanical, u->getType().isMechanical() );
    U_UFILTER(IsRobotic, u->getType().isRobotic() );
    U_UFILTER(IsDetector, u->getType().isDetector() );
    U_UFILTER(IsResourceContainer, u->getType().isResourceContainer() );
    U_UFILTER(IsResourceDepot, u->getType().isResourceDepot() );
    U_UFILTER(IsRefinery, u->getType().isRefinery() );
    U_UFILTER(IsWorker, u->getType().isWorker() );
    U_UFILTER(RequiresPsi, u->getType().requiresPsi() );
    U_UFILTER(RequiresCreep, u->getType().requiresCreep() );
    U_UFILTER(IsBurrowable, u->getType().isBurrowable() );
    U_UFILTER(IsCloakable, u->getType().isCloakable() );
    U_UFILTER(IsBuilding, u->getType().isBuilding() );
    U_UFILTER(IsAddon, u->getType().isAddon() );
    U_UFILTER(IsFlyingBuilding, u->getType().isFlyingBuilding() );
    U_UFILTER(IsNeutral, u->getType().isNeutral() );
    U_UFILTER(IsHero, u->getType().isHero() );
    U_UFILTER(IsPowerup, u->getType().isPowerup() );
    U_UFILTER(IsBeacon, u->getType().isBeacon() );
    U_UFILTER(IsFlagBeacon, u->getType().isFlagBeacon() );
    U_UFILTER(IsSpecialBuilding, u->getType().isSpecialBuilding() );
    U_UFILTER(IsSpell, u->getType().isSpell() );
    U_UFILTER(ProducesLarva, u->getType().producesLarva() );
    U_UFILTER(IsMineralField, u->getType().isMineralField() );
    U_UFILTER(IsCritter, u->getType().isCritter() );
    U_UFILTER(CanBuildAddon, u->getType().canBuildAddon() );
  
    U_CFILTER(HP, u->getHitPoints() );
    U_CFILTER(MaxHP, u->getType().maxHitPoints() );
    U_CFILTER(HP_Percent, (u->getType().maxHitPoints() != 0) ? ((u->getHitPoints()*100) / u->getType().maxHitPoints()) : 0 );
  
    U_CFILTER(Shields, u->getShields() );
    U_CFILTER(MaxShields, u->getType().maxShields() );
    U_CFILTER(Shields_Percent, (u->getType().maxShields() != 0) ? ((u->getShields()*100) / u->getType().maxShields()) : 0 );
  
    U_CFILTER(Energy, u->getEnergy() );
    U_CFILTER(MaxEnergy, u->getPlayer()->maxEnergy(u->getType()) );
    U_CFILTER(Energy_Percent, (u->getPlayer()->maxEnergy(u->getType()) != 0) ? ((u->getEnergy()*100) / u->getPlayer()->maxEnergy(u->getType())) : 0 );

    U_CFILTER(Armor, u->getPlayer()->armor(u->getType()) );
    U_CFILTERX(ArmorUpgrade, u->getType().armorUpgrade(), UpgradeType );

    U_CFILTER(MineralPrice, u->getType().mineralPrice() );
    U_CFILTER(GasPrice, u->getType().gasPrice() );
    U_CFILTER(BuildTime, u->getType().buildTime() );

    U_CFILTER(SupplyRequired, u->getType().supplyRequired() );
    U_CFILTER(SupplyProvided, u->getType().supplyProvided() );

    U_CFILTER(SpaceRequired, u->getType().spaceRequired() );
    U_CFILTER(SpaceRemaining, u->getSpaceRemaining() );
    U_CFILTER(SpaceProvided, u->getType().spaceProvided() );

    U_CFILTER(BuildScore, u->getType().buildScore() );
    U_CFILTER(DestroyScore, u->getType().destroyScore() );

    U_CFILTERX(TopSpeed, u->getPlayer()->topSpeed(u->getType()), double );
    U_CFILTER(SightRange, u->getPlayer()->sightRange(u->getType()) );
    U_CFILTER(MaxWeaponCooldown, u->getPlayer()->weaponDamageCooldown(u->getType()) );
    U_CFILTERX(SizeType, u->getType().size(), UnitSizeType );

    U_CFILTERX(GroundWeapon, u->getType().groundWeapon(), WeaponType );
    U_CFILTERX(AirWeapon, u->getType().airWeapon(), WeaponType );

    U_CFILTERX(GetType, u->getType(), UnitType );
    U_CFILTERX(GetRace, u->getType().getRace(), Race );
    U_CFILTERX(GetPlayer, u->getPlayer(), Player );
  
    U_CFILTER(Resources, u->getResources() );
    U_CFILTER(ResourceGroup, u->getResourceGroup() );
    U_CFILTER(AcidSporeCount, u->getAcidSporeCount() );
    U_CFILTER(InterceptorCount, u->getInterceptorCount() );
    U_CFILTER(ScarabCount, u->getScarabCount() );
    U_CFILTER(SpiderMineCount, u->getSpiderMineCount() );
    U_CFILTER(WeaponCooldown, u->getGroundWeaponCooldown() );
    U_CFILTER(SpellCooldown, u->getSpellCooldown() );

    U_CFILTER(DefenseMatrixPoints, u->getDefenseMatrixPoints() );
    U_CFILTER(DefenseMatrixTime, u->getDefenseMatrixTimer() );
    U_CFILTER(EnsnareTime, u->getEnsnareTimer() );
    U_CFILTER(IrradiateTime, u->getIrradiateTimer() );
    U_CFILTER(LockdownTime, u->getLockdownTimer() );
    U_CFILTER(MaelstromTime, u->getMaelstromTimer() );
    U_CFILTER(OrderTime, u->getOrderTimer() );
    U_CFILTER(PlagueTimer, u->getPlagueTimer() );
    U_CFILTER(RemoveTime, u->getRemoveTimer() );
    U_CFILTER(StasisTime, u->getStasisTimer() );
    U_CFILTER(StimTime, u->getStimTimer() );
    U_CFILTERX(BuildType, u->getBuildType(), UnitType );
    U_CFILTER(RemainingBuildTime, u->getRemainingBuildTime() );
    U_CFILTER(RemainingTrainTime, u->getRemainingTrainTime() );
    U_CFILTERX(Target, u->getTarget(), Unit );
    U_CFILTERX(CurrentOrder, u->getOrder(), Order );
    U_CFILTERX(SecondaryOrder, u->getSecondaryOrder(), Order );
    U_CFILTERX(OrderTarget, u->getOrderTarget(), Unit );
    U_CFILTER(GetLeft, u->getLeft() );
    U_CFILTER(GetTop, u->getTop() );
    U_CFILTER(GetRight, u->getRight() );
    U_CFILTER(GetBottom, u->getBottom() );

    U_UFILTER(Exists, u->exists() );
    U_UFILTER(IsAttacking, u->isAttacking() );
    U_UFILTER(IsBeingConstructed, u->isBeingConstructed() );
    U_UFILTER(IsBeingGathered, u->isBeingGathered() );
    U_UFILTER(IsBeingHealed, u->isBeingHealed() );
    U_UFILTER(IsBlind, u->isBlind() );
    U_UFILTER(IsBraking, u->isBraking() );
    U_UFILTER(IsBurrowed, u->isBurrowed() );
    U_UFILTER(IsCarryingGas, u->isCarryingGas() );
    U_UFILTER(IsCarryingMinerals, u->isCarryingMinerals() );
    U_UFILTER(IsCarryingSomething, u->isCarryingMinerals() || u->isCarryingGas() );
    U_UFILTER(IsCloaked, u->isCloaked() );
    U_UFILTER(IsCompleted, u->isCompleted() );
    U_UFILTER(IsConstructing, u->isConstructing() );
    U_UFILTER(IsDefenseMatrixed, u->isDefenseMatrixed() );
    U_UFILTER(IsDetected, u->isDetected() );
    U_UFILTER(IsEnsnared, u->isEnsnared() );
    U_UFILTER(IsFollowing, u->isFollowing() );
    U_UFILTER(IsGatheringGas, u->isGatheringGas() );
    U_UFILTER(IsGatheringMinerals, u->isGatheringMinerals() );
    U_UFILTER(IsHallucination, u->isHallucination() );
    U_UFILTER(IsHoldingPosition, u->isHoldingPosition() );
    U_UFILTER(IsIdle, u->isIdle() );
    U_UFILTER(IsInterruptible, u->isInterruptible() );
    U_UFILTER(IsInvincible, u->isInvincible() );
    U_UFILTER(IsIrradiated, u->isIrradiated() );
    U_UFILTER(IsLifted, u->isLifted() );
    U_UFILTER(IsLoaded, u->isLoaded() );
    U_UFILTER(IsLockedDown, u->isLockedDown() );
    U_UFILTER(IsMaelstrommed, u->isMaelstrommed() );
    U_UFILTER(IsMorphing, u->isMorphing() );
    U_UFILTER(IsMoving, u->isMoving() );
    U_UFILTER(IsParasited, u->isParasited() );
    U_UFILTER(IsPatrolling, u->isPatrolling() );
    U_UFILTER(IsPlagued, u->isPlagued() );
    U_UFILTER(IsRepairing, u->isRepairing() );
    U_UFILTER(IsResearching, u->isResearching() );
    U_UFILTER(IsSieged, u->isSieged() );
    U_UFILTER(IsStartingAttack, u->isStartingAttack() );
    U_UFILTER(IsStasised, u->isStasised() );
    U_UFILTER(IsStimmed, u->isStimmed() );
    U_UFILTER(IsStuck, u->isStuck() );
    U_UFILTER(IsTraining, u->isTraining() );
    U_UFILTER(IsUnderAttack, u->isUnderAttack() );
    U_UFILTER(IsUnderDarkSwarm, u->isUnderDarkSwarm() );
    U_UFILTER(IsUnderDisruptionWeb, u->isUnderDisruptionWeb() );
    U_UFILTER(IsUnderStorm, u->isUnderStorm() );
    U_UFILTER(IsPowered, u->isPowered() );
    U_UFILTER(IsVisible, u->isVisible() );

    bool IsEnemyImpl(const Unit &u)
    {
      auto const self = u->getGame().self();
      return self == nullptr ? false : self->isEnemy(u->getPlayer());
    }
    const PtrUnitFilter IsEnemy( &IsEnemyImpl );

    bool IsAllyImpl(const Unit &u)
    {
      auto const self = u->getGame().self();
      return self == nullptr ? false : self->isAlly( u->getPlayer() );
    }
    const PtrUnitFilter IsAlly( &IsAllyImpl );

    U_UFILTER(IsOwned, u->getPlayer() == u->getGame().self() );
  }
}

