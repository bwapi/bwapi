#pragma once
#include <BWAPI/UnaryFilter.h>
#include <BWAPI/ComparisonFilter.h>
#include <BWAPI/BestFilter.h>
#include <BWAPI/UnitSizeType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/WeaponType.h>
#include <BWAPI/Order.h>
#include <BWAPI/Race.h>

namespace BWAPI
{
  // forwards
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class UnitType;
  class UnitInterface;
  typedef UnitInterface *Unit;

  // typedefs
  typedef UnaryFilter<Unit > UnitFilter;
  typedef UnaryFilter<Unit ,bool (*)(Unit )> PtrUnitFilter;
  typedef BestFilter<Unit >  BestUnitFilter;
  typedef CompareFilter<Unit ,int,int(*)(Unit )> PtrIntCompareUnitFilter;

  namespace Filter
  {
    /// <summary>A unary filter that checks if the unit type is a transport.</summary>
    extern const PtrUnitFilter IsTransport;

    /// @see BWAPI::UnitType::canProduce
    extern const PtrUnitFilter CanProduce;

    /// @see BWAPI::UnitType::canAttack
    extern const PtrUnitFilter CanAttack;

    /// @see BWAPI::UnitType::canMove
    extern const PtrUnitFilter CanMove;

    /// @see BWAPI::UnitType::isFlying
    extern const PtrUnitFilter IsFlyer;

    /// @see BWAPI::UnitInterface::IsFlying
    extern const PtrUnitFilter IsFlying;

    /// @see BWAPI::UnitType::regeneratesHP
    extern const PtrUnitFilter RegeneratesHP;

    /// @see BWAPI::UnitType::isSpellcaster
    extern const PtrUnitFilter IsSpellcaster;
    
    /// @see BWAPI::UnitType::hasPermanentCloak
    extern const PtrUnitFilter HasPermanentCloak;

    /// @see BWAPI::UnitType::isOrganic
    extern const PtrUnitFilter IsOrganic;

    /// @see BWAPI::UnitType::isMechanical
    extern const PtrUnitFilter IsMechanical;

    /// @see BWAPI::UnitType::isRobotic
    extern const PtrUnitFilter IsRobotic;

    /// @see BWAPI::UnitType::isDetector
    extern const PtrUnitFilter IsDetector;

    /// @see BWAPI::UnitType::isResourceContainer
    extern const PtrUnitFilter IsResourceContainer;

    /// @see BWAPI::UnitType::isResourceDepot
    extern const PtrUnitFilter IsResourceDepot;

    /// @see BWAPI::UnitType::isRefinery
    extern const PtrUnitFilter IsRefinery;

    /// @see BWAPI::UnitType::isWorker
    extern const PtrUnitFilter IsWorker;

    /// @see BWAPI::UnitType::requiresPsi
    extern const PtrUnitFilter RequiresPsi;

    /// @see BWAPI::UnitType::requiresCreep
    extern const PtrUnitFilter RequiresCreep;

    /// @see BWAPI::UnitType::isBurrowable
    extern const PtrUnitFilter IsBurrowable;

    /// @see BWAPI::UnitType::isCloakable
    extern const PtrUnitFilter IsCloakable;

    /// @see BWAPI::UnitType::isBuilding
    extern const PtrUnitFilter IsBuilding;

    /// @see BWAPI::UnitType::isAddon
    extern const PtrUnitFilter IsAddon;

    /// @see BWAPI::UnitType::isFlyingBuilding
    extern const PtrUnitFilter IsFlyingBuilding;

    /// @see BWAPI::UnitType::isNeutral
    extern const PtrUnitFilter IsNeutral;

    /// @see BWAPI::UnitType::isHero
    extern const PtrUnitFilter IsHero;

    /// @see BWAPI::UnitType::isPowerup
    extern const PtrUnitFilter IsPowerup;

    /// @see BWAPI::UnitType::isBeacon
    extern const PtrUnitFilter IsBeacon;

    /// @see BWAPI::UnitType::isFlagBeacon
    extern const PtrUnitFilter IsFlagBeacon;

    /// @see BWAPI::UnitType::isSpecialBuilding
    extern const PtrUnitFilter IsSpecialBuilding;

    /// @see BWAPI::UnitType::isSpell
    extern const PtrUnitFilter IsSpell;

    /// @see BWAPI::UnitType::producesLarva
    extern const PtrUnitFilter ProducesLarva;

    /// @see BWAPI::UnitType::isMineralField
    extern const PtrUnitFilter IsMineralField;

    /// @see BWAPI::UnitType::isCritter
    extern const PtrUnitFilter IsCritter;

    /// @see BWAPI::UnitType::canBuildAddon
    extern const PtrUnitFilter CanBuildAddon;
  
    /// @see BWAPI::UnitInterface::getHitPoints
    extern const PtrIntCompareUnitFilter HP;

    /// @see BWAPI::UnitType::maxHitPoints
    extern const PtrIntCompareUnitFilter MaxHP;

    /// <summary>A comparison filter that retrieves the unit's HP percentage.</summary> The formula
    /// is HP*100/MaxHP.
    extern const PtrIntCompareUnitFilter HP_Percent;
  
    /// @see BWAPI::UnitInterface::getShields
    extern const PtrIntCompareUnitFilter Shields;

    /// @see BWAPI::UnitType::maxShields
    extern const PtrIntCompareUnitFilter MaxShields;

    /// <summary>A comparison filter that retrieves the unit's shields percentage.</summary> The
    /// formula is Shields*100/MaxShields.
    extern const PtrIntCompareUnitFilter Shields_Percent;
  
    /// @see BWAPI::UnitInterface::getEnergy
    extern const PtrIntCompareUnitFilter Energy;
    
    /// @see BWAPI::PlayerInterface::maxEnergy
    extern const PtrIntCompareUnitFilter MaxEnergy;
    
    /// <summary>A comparison filter that retrieves the unit's energy percentage.</summary> The
    /// formula is Energy*100/MaxEnergy.
    extern const PtrIntCompareUnitFilter Energy_Percent;

    /// @see BWAPI::PlayerInterface::armor
    extern const PtrIntCompareUnitFilter Armor;

    /// @see BWAPI::UnitType::armorUpgrade
    extern const CompareFilter<Unit ,UpgradeType,UpgradeType (*)(Unit )> ArmorUpgrade;

    /// @see BWAPI::UnitType::mineralPrice
    extern const PtrIntCompareUnitFilter MineralPrice;

    /// @see BWAPI::UnitType::gasPrice
    extern const PtrIntCompareUnitFilter GasPrice;

    /// @see BWAPI::UnitType::buildTime
    extern const PtrIntCompareUnitFilter BuildTime;

    /// @see BWAPI::UnitType::supplyRequired
    extern const PtrIntCompareUnitFilter SupplyRequired;

    /// @see BWAPI::UnitType::supplyProvided
    extern const PtrIntCompareUnitFilter SupplyProvided;

    /// @see BWAPI::UnitType::spaceRequired
    extern const PtrIntCompareUnitFilter SpaceRequired;

    /// @see BWAPI::UnitInterface::getSpaceRemaining
    extern const PtrIntCompareUnitFilter SpaceRemaining;

    /// @see BWAPI::UnitType::spaceProvided
    extern const PtrIntCompareUnitFilter SpaceProvided;

    /// @see BWAPI::UnitType::buildScore
    extern const PtrIntCompareUnitFilter BuildScore;

    /// @see BWAPI::UnitType::destroyScore
    extern const PtrIntCompareUnitFilter DestroyScore;

    /// @see BWAPI::PlayerInterface::topSpeed
    extern const CompareFilter<Unit ,double,double (*)(Unit )> TopSpeed;

    /// @see BWAPI::PlayerInterface::sightRange
    extern const PtrIntCompareUnitFilter SightRange;

    /// @see BWAPI::PlayerInterface::weaponDamageCooldown
    extern const PtrIntCompareUnitFilter WeaponCooldown;

    /// @see BWAPI::UnitType::size
    extern const CompareFilter<Unit ,UnitSizeType,UnitSizeType (*)(Unit )> SizeType;

    /// @see BWAPI::UnitType::groundWeapon
    extern const CompareFilter<Unit ,WeaponType,WeaponType (*)(Unit )> GroundWeapon;

    /// @see BWAPI::UnitType::airWeapon
    extern const CompareFilter<Unit ,WeaponType,WeaponType (*)(Unit )> AirWeapon;

    /// @see BWAPI::UnitInterface::getType
    extern const CompareFilter<Unit ,UnitType,UnitType (*)(Unit )> GetType;

    /// @see BWAPI::UnitType::getRace
    extern const CompareFilter<Unit ,Race,Race (*)(Unit )> GetRace;

    /// @see BWAPI::UnitInterface::getPlayer
    extern const CompareFilter<Unit ,Player,Player (*)(Unit )> GetPlayer;
  
    /// @see BWAPI::UnitInterface::getResources
    extern const PtrIntCompareUnitFilter Resources;

    /// @see BWAPI::UnitInterface::getResourceGroup
    extern const PtrIntCompareUnitFilter ResourceGroup;

    /// @see BWAPI::UnitInterface::getAcidSporeCount
    extern const PtrIntCompareUnitFilter AcidSporeCount;

    /// @see BWAPI::UnitInterface::getInterceptorCount
    extern const PtrIntCompareUnitFilter InterceptorCount;

    /// @see BWAPI::UnitInterface::getScarabCount
    extern const PtrIntCompareUnitFilter ScarabCount;

    /// @see BWAPI::UnitInterface::getSpiderMineCount
    extern const PtrIntCompareUnitFilter SpiderMineCount;

    /// @see BWAPI::UnitInterface::getGroundWeaponCooldown
    extern const PtrIntCompareUnitFilter MaxWeaponCooldown;

    /// @see BWAPI::UnitInterface::getSpellCooldown
    extern const PtrIntCompareUnitFilter SpellCooldown;

    /// @see BWAPI::UnitInterface::getDefenseMatrixPoints
    extern const PtrIntCompareUnitFilter DefenseMatrixPoints;

    /// @see BWAPI::UnitInterface::getDefenseMatrixTimer
    extern const PtrIntCompareUnitFilter DefenseMatrixTime;

    /// @see BWAPI::UnitInterface::getEnsnareTimer
    extern const PtrIntCompareUnitFilter EnsnareTime;

    /// @see BWAPI::UnitInterface::getIrradiateTimer
    extern const PtrIntCompareUnitFilter IrradiateTime;

    /// @see BWAPI::UnitInterface::getLockdownTimer
    extern const PtrIntCompareUnitFilter LockdownTime;

    /// @see BWAPI::UnitInterface::getMaelstromTimer
    extern const PtrIntCompareUnitFilter MaelstromTime;

    /// @see BWAPI::UnitInterface::getOrderTimer
    extern const PtrIntCompareUnitFilter OrderTime;

    /// @see BWAPI::UnitInterface::getPlagueTimer
    extern const PtrIntCompareUnitFilter PlagueTimer;

    /// @see BWAPI::UnitInterface::getRemoveTimer
    extern const PtrIntCompareUnitFilter RemoveTime;

    /// @see BWAPI::UnitInterface::getStasisTimer
    extern const PtrIntCompareUnitFilter StasisTime;

    /// @see BWAPI::UnitInterface::getStimTimer
    extern const PtrIntCompareUnitFilter StimTime;

    /// @see BWAPI::UnitInterface::getBuildType
    extern const CompareFilter<Unit ,UnitType,UnitType (*)(Unit )> BuildType;

    /// @see BWAPI::UnitInterface::getRemainingBuildTime
    extern const PtrIntCompareUnitFilter RemainingBuildTime;

    /// @see BWAPI::UnitInterface::getRemainingTrainTime
    extern const PtrIntCompareUnitFilter RemainingTrainTime;

    /// @see BWAPI::UnitInterface::getTarget
    extern const CompareFilter<Unit ,Unit ,Unit (*)(Unit )> Target;

    /// @see BWAPI::UnitInterface::getOrder
    extern const CompareFilter<Unit ,Order,Order (*)(Unit )> CurrentOrder;

    /// @see BWAPI::UnitInterface::getSecondaryOrder
    extern const CompareFilter<Unit ,Order,Order (*)(Unit )> SecondaryOrder;

    /// @see BWAPI::UnitInterface::getOrderTarget
    extern const CompareFilter<Unit ,Unit ,Unit (*)(Unit )> OrderTarget;

    /// @see BWAPI::UnitInterface::getLeft
    extern const PtrIntCompareUnitFilter GetLeft;

    /// @see BWAPI::UnitInterface::getTop
    extern const PtrIntCompareUnitFilter GetTop;

    /// @see BWAPI::UnitInterface::getRight
    extern const PtrIntCompareUnitFilter GetRight;

    /// @see BWAPI::UnitInterface::getBottom
    extern const PtrIntCompareUnitFilter GetBottom;

    /// @see BWAPI::UnitInterface::exists
    extern const PtrUnitFilter Exists;

    /// @see BWAPI::UnitInterface::isAttacking
    extern const PtrUnitFilter IsAttacking;

    /// @see BWAPI::UnitInterface::isBeingConstructed
    extern const PtrUnitFilter IsBeingConstructed;

    /// @see BWAPI::UnitInterface::isBeingGathered
    extern const PtrUnitFilter IsBeingGathered;

    /// @see BWAPI::UnitInterface::isBeingHealed
    extern const PtrUnitFilter IsBeingHealed;

    /// @see BWAPI::UnitInterface::isBlind
    extern const PtrUnitFilter IsBlind;

    /// @see BWAPI::UnitInterface::isBraking
    extern const PtrUnitFilter IsBraking;

    /// @see BWAPI::UnitInterface::isBurrowed
    extern const PtrUnitFilter IsBurrowed;

    /// @see BWAPI::UnitInterface::isCarryingGas
    extern const PtrUnitFilter IsCarryingGas;

    /// @see BWAPI::UnitInterface::isCarryingMinerals
    extern const PtrUnitFilter IsCarryingMinerals;

    /// <summary>A unary filter that checks if IsCarryingGas or IsCarryingMinerals return true.</summary>
    extern const PtrUnitFilter IsCarryingSomething;

    /// @see BWAPI::UnitInterface::isCloaked
    extern const PtrUnitFilter IsCloaked;

    /// @see BWAPI::UnitInterface::isCompleted
    extern const PtrUnitFilter IsCompleted;

    /// @see BWAPI::UnitInterface::isConstructing
    extern const PtrUnitFilter IsConstructing;

    /// @see BWAPI::UnitInterface::isDefenseMatrixed
    extern const PtrUnitFilter IsDefenseMatrixed;

    /// @see BWAPI::UnitInterface::isDetected
    extern const PtrUnitFilter IsDetected;

    /// @see BWAPI::UnitInterface::isEnsnared
    extern const PtrUnitFilter IsEnsnared;

    /// @see BWAPI::UnitInterface::isFollowing
    extern const PtrUnitFilter IsFollowing;

    /// @see BWAPI::UnitInterface::isGatheringGas
    extern const PtrUnitFilter IsGatheringGas;

    /// @see BWAPI::UnitInterface::isGatheringMinerals
    extern const PtrUnitFilter IsGatheringMinerals;

    /// @see BWAPI::UnitInterface::isHallucination
    extern const PtrUnitFilter IsHallucination;

    /// @see BWAPI::UnitInterface::isHoldingPosition
    extern const PtrUnitFilter IsHoldingPosition;

    /// @see BWAPI::UnitInterface::isIdle
    extern const PtrUnitFilter IsIdle;

    /// @see BWAPI::UnitInterface::isInterruptible
    extern const PtrUnitFilter IsInterruptible;

    /// @see BWAPI::UnitInterface::isInvincible
    extern const PtrUnitFilter IsInvincible;

    /// @see BWAPI::UnitInterface::isIrradiated
    extern const PtrUnitFilter IsIrradiated;

    /// @see BWAPI::UnitInterface::isLifted
    extern const PtrUnitFilter IsLifted;

    /// @see BWAPI::UnitInterface::isLoaded
    extern const PtrUnitFilter IsLoaded;

    /// @see BWAPI::UnitInterface::isLockedDown
    extern const PtrUnitFilter IsLockedDown;

    /// @see BWAPI::UnitInterface::isMaelstrommed
    extern const PtrUnitFilter IsMaelstrommed;

    /// @see BWAPI::UnitInterface::isMorphing
    extern const PtrUnitFilter IsMorphing;

    /// @see BWAPI::UnitInterface::isMoving
    extern const PtrUnitFilter IsMoving;

    /// @see BWAPI::UnitInterface::isParasited
    extern const PtrUnitFilter IsParasited;

    /// @see BWAPI::UnitInterface::isPatrolling
    extern const PtrUnitFilter IsPatrolling;

    /// @see BWAPI::UnitInterface::isPlagued
    extern const PtrUnitFilter IsPlagued;

    /// @see BWAPI::UnitInterface::isRepairing
    extern const PtrUnitFilter IsRepairing;

    /// @see BWAPI::UnitInterface::isResearching
    extern const PtrUnitFilter IsResearching;

    /// @see BWAPI::UnitInterface::isSieged
    extern const PtrUnitFilter IsSieged;

    /// @see BWAPI::UnitInterface::isStartingAttack
    extern const PtrUnitFilter IsStartingAttack;

    /// @see BWAPI::UnitInterface::isStasised
    extern const PtrUnitFilter IsStasised;

    /// @see BWAPI::UnitInterface::isStimmed
    extern const PtrUnitFilter IsStimmed;

    /// @see BWAPI::UnitInterface::isStuck
    extern const PtrUnitFilter IsStuck;

    /// @see BWAPI::UnitInterface::isTraining
    extern const PtrUnitFilter IsTraining;

    /// @see BWAPI::UnitInterface::isUnderAttack
    extern const PtrUnitFilter IsUnderAttack;

    /// @see BWAPI::UnitInterface::isUnderDarkSwarm
    extern const PtrUnitFilter IsUnderDarkSwarm;

    /// @see BWAPI::UnitInterface::isUnderDisruptionWeb
    extern const PtrUnitFilter IsUnderDisruptionWeb;

    /// @see BWAPI::UnitInterface::isUnderStorm
    extern const PtrUnitFilter IsUnderStorm;

    /// @see BWAPI::UnitInterface::isPowered
    extern const PtrUnitFilter IsPowered;

    /// @see BWAPI::UnitInterface::isVisible
    extern const PtrUnitFilter IsVisible;
  
    /// <summary>A unary filter that checks if the current unit is an enemy of the BWAPI player.</summary>
    /// @note This will always be false when the BWAPI Player is unspecified, such as in a replay.
    extern const PtrUnitFilter IsEnemy;

    /// <summary>A unary filter that checks if the current unit is an ally of the BWAPI player.</summary>
    /// @note This will always be false when the BWAPI Player is unspecified, such as in a replay.
    extern const PtrUnitFilter IsAlly;

    /// <summary>A unary filter that checks if the current unit is owned by the BWAPI player.</summary>
    /// @note This will always be false when the BWAPI Player is unspecified, such as in a replay.
    extern const PtrUnitFilter IsOwned;
  }
}

