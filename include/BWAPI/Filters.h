#pragma once

#include <BWAPI/UnaryFilter.h>
#include <BWAPI/ComparisonFilter.h>
#include <BWAPI/BestFilter.h>

namespace BWAPI
{
  class Unit;
  class UpgradeType;
  class UnitType;
  class Player;
  class UnitSizeType;
  class Race;
  class WeaponType;
  class Order;

  // typedefs
  using UnitFilter = UnaryFilter<Unit>;
  using PtrUnitFilter = UnaryFilter<Unit, bool(*)(Unit const &)>;
  using BestUnitFilter = BestFilter<Unit>;
  
  template<typename T>
  using PtrCompareUnitFilter = CompareFilter<Unit, T, T(*)(Unit const &)>;

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

    /// @see BWAPI::Unit::IsFlying
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
  
    /// @see BWAPI::Unit::getHitPoints
    extern const PtrCompareUnitFilter<int> HP;

    /// @see BWAPI::UnitType::maxHitPoints
    extern const PtrCompareUnitFilter<int> MaxHP;

    /// <summary>A comparison filter that retrieves the unit's HP percentage.</summary> The formula
    /// is HP*100/MaxHP.
    extern const PtrCompareUnitFilter<int> HP_Percent;
  
    /// @see BWAPI::Unit::getShields
    extern const PtrCompareUnitFilter<int> Shields;

    /// @see BWAPI::UnitType::maxShields
    extern const PtrCompareUnitFilter<int> MaxShields;

    /// <summary>A comparison filter that retrieves the unit's shields percentage.</summary> The
    /// formula is Shields*100/MaxShields.
    extern const PtrCompareUnitFilter<int> Shields_Percent;
  
    /// @see BWAPI::Unit::getEnergy
    extern const PtrCompareUnitFilter<int> Energy;
    
    /// @see BWAPI::Player::maxEnergy
    extern const PtrCompareUnitFilter<int> MaxEnergy;
    
    /// <summary>A comparison filter that retrieves the unit's energy percentage.</summary> The
    /// formula is Energy*100/MaxEnergy.
    extern const PtrCompareUnitFilter<int> Energy_Percent;

    /// @see BWAPI::Player::armor
    extern const PtrCompareUnitFilter<int> Armor;

    /// @see BWAPI::UnitType::armorUpgrade
    extern const PtrCompareUnitFilter<UpgradeType> ArmorUpgrade;

    /// @see BWAPI::UnitType::mineralPrice
    extern const PtrCompareUnitFilter<int> MineralPrice;

    /// @see BWAPI::UnitType::gasPrice
    extern const PtrCompareUnitFilter<int> GasPrice;

    /// @see BWAPI::UnitType::buildTime
    extern const PtrCompareUnitFilter<int> BuildTime;

    /// @see BWAPI::UnitType::supplyRequired
    extern const PtrCompareUnitFilter<int> SupplyRequired;

    /// @see BWAPI::UnitType::supplyProvided
    extern const PtrCompareUnitFilter<int> SupplyProvided;

    /// @see BWAPI::UnitType::spaceRequired
    extern const PtrCompareUnitFilter<int> SpaceRequired;

    /// @see BWAPI::Unit::getSpaceRemaining
    extern const PtrCompareUnitFilter<int> SpaceRemaining;

    /// @see BWAPI::UnitType::spaceProvided
    extern const PtrCompareUnitFilter<int> SpaceProvided;

    /// @see BWAPI::UnitType::buildScore
    extern const PtrCompareUnitFilter<int> BuildScore;

    /// @see BWAPI::UnitType::destroyScore
    extern const PtrCompareUnitFilter<int> DestroyScore;

    /// @see BWAPI::Player::topSpeed
    extern const PtrCompareUnitFilter<double> TopSpeed;

    /// @see BWAPI::Player::sightRange
    extern const PtrCompareUnitFilter<int> SightRange;

    /// @see BWAPI::Player::weaponDamageCooldown
    extern const PtrCompareUnitFilter<int> WeaponCooldown;

    /// @see BWAPI::UnitType::size
    extern const PtrCompareUnitFilter<UnitSizeType> SizeType;

    /// @see BWAPI::UnitType::groundWeapon
    extern const PtrCompareUnitFilter<WeaponType> GroundWeapon;

    /// @see BWAPI::UnitType::airWeapon
    extern const PtrCompareUnitFilter<WeaponType> AirWeapon;

    /// @see BWAPI::Unit::getType
    extern const PtrCompareUnitFilter<UnitType> GetType;

    /// @see BWAPI::UnitType::getRace
    extern const PtrCompareUnitFilter<Race> GetRace;

    /// @see BWAPI::Unit::getPlayer
    extern const PtrCompareUnitFilter<Player> GetPlayer;
  
    /// @see BWAPI::Unit::getResources
    extern const PtrCompareUnitFilter<int> Resources;

    /// @see BWAPI::Unit::getResourceGroup
    extern const PtrCompareUnitFilter<int> ResourceGroup;

    /// @see BWAPI::Unit::getAcidSporeCount
    extern const PtrCompareUnitFilter<int> AcidSporeCount;

    /// @see BWAPI::Unit::getInterceptorCount
    extern const PtrCompareUnitFilter<int> InterceptorCount;

    /// @see BWAPI::Unit::getScarabCount
    extern const PtrCompareUnitFilter<int> ScarabCount;

    /// @see BWAPI::Unit::getSpiderMineCount
    extern const PtrCompareUnitFilter<int> SpiderMineCount;

    /// @see BWAPI::Unit::getGroundWeaponCooldown
    extern const PtrCompareUnitFilter<int> MaxWeaponCooldown;

    /// @see BWAPI::Unit::getSpellCooldown
    extern const PtrCompareUnitFilter<int> SpellCooldown;

    /// @see BWAPI::Unit::getDefenseMatrixPoints
    extern const PtrCompareUnitFilter<int> DefenseMatrixPoints;

    /// @see BWAPI::Unit::getDefenseMatrixTimer
    extern const PtrCompareUnitFilter<int> DefenseMatrixTime;

    /// @see BWAPI::Unit::getEnsnareTimer
    extern const PtrCompareUnitFilter<int> EnsnareTime;

    /// @see BWAPI::Unit::getIrradiateTimer
    extern const PtrCompareUnitFilter<int> IrradiateTime;

    /// @see BWAPI::Unit::getLockdownTimer
    extern const PtrCompareUnitFilter<int> LockdownTime;

    /// @see BWAPI::Unit::getMaelstromTimer
    extern const PtrCompareUnitFilter<int> MaelstromTime;

    /// @see BWAPI::Unit::getOrderTimer
    extern const PtrCompareUnitFilter<int> OrderTime;

    /// @see BWAPI::Unit::getPlagueTimer
    extern const PtrCompareUnitFilter<int> PlagueTimer;

    /// @see BWAPI::Unit::getRemoveTimer
    extern const PtrCompareUnitFilter<int> RemoveTime;

    /// @see BWAPI::Unit::getStasisTimer
    extern const PtrCompareUnitFilter<int> StasisTime;

    /// @see BWAPI::Unit::getStimTimer
    extern const PtrCompareUnitFilter<int> StimTime;

    /// @see BWAPI::Unit::getBuildType
    extern const PtrCompareUnitFilter<UnitType> BuildType;

    /// @see BWAPI::Unit::getRemainingBuildTime
    extern const PtrCompareUnitFilter<int> RemainingBuildTime;

    /// @see BWAPI::Unit::getRemainingTrainTime
    extern const PtrCompareUnitFilter<int> RemainingTrainTime;

    /// @see BWAPI::Unit::getTarget
    extern const PtrCompareUnitFilter<Unit> Target;

    /// @see BWAPI::Unit::getOrder
    extern const PtrCompareUnitFilter<Order> CurrentOrder;

    /// @see BWAPI::Unit::getSecondaryOrder
    extern const PtrCompareUnitFilter<Order> SecondaryOrder;

    /// @see BWAPI::Unit::getOrderTarget
    extern const PtrCompareUnitFilter<Unit> OrderTarget;

    /// @see BWAPI::Unit::getLeft
    extern const PtrCompareUnitFilter<int> GetLeft;

    /// @see BWAPI::Unit::getTop
    extern const PtrCompareUnitFilter<int> GetTop;

    /// @see BWAPI::Unit::getRight
    extern const PtrCompareUnitFilter<int> GetRight;

    /// @see BWAPI::Unit::getBottom
    extern const PtrCompareUnitFilter<int> GetBottom;

    /// @see BWAPI::Unit::exists
    extern const PtrUnitFilter Exists;

    /// @see BWAPI::Unit::isAttacking
    extern const PtrUnitFilter IsAttacking;

    /// @see BWAPI::Unit::isBeingConstructed
    extern const PtrUnitFilter IsBeingConstructed;

    /// @see BWAPI::Unit::isBeingGathered
    extern const PtrUnitFilter IsBeingGathered;

    /// @see BWAPI::Unit::isBeingHealed
    extern const PtrUnitFilter IsBeingHealed;

    /// @see BWAPI::Unit::isBlind
    extern const PtrUnitFilter IsBlind;

    /// @see BWAPI::Unit::isBraking
    extern const PtrUnitFilter IsBraking;

    /// @see BWAPI::Unit::isBurrowed
    extern const PtrUnitFilter IsBurrowed;

    /// @see BWAPI::Unit::isCarryingGas
    extern const PtrUnitFilter IsCarryingGas;

    /// @see BWAPI::Unit::isCarryingMinerals
    extern const PtrUnitFilter IsCarryingMinerals;

    /// <summary>A unary filter that checks if IsCarryingGas or IsCarryingMinerals return true.</summary>
    extern const PtrUnitFilter IsCarryingSomething;

    /// @see BWAPI::Unit::isCloaked
    extern const PtrUnitFilter IsCloaked;

    /// @see BWAPI::Unit::isCompleted
    extern const PtrUnitFilter IsCompleted;

    /// @see BWAPI::Unit::isConstructing
    extern const PtrUnitFilter IsConstructing;

    /// @see BWAPI::Unit::isDefenseMatrixed
    extern const PtrUnitFilter IsDefenseMatrixed;

    /// @see BWAPI::Unit::isDetected
    extern const PtrUnitFilter IsDetected;

    /// @see BWAPI::Unit::isEnsnared
    extern const PtrUnitFilter IsEnsnared;

    /// @see BWAPI::Unit::isFollowing
    extern const PtrUnitFilter IsFollowing;

    /// @see BWAPI::Unit::isGatheringGas
    extern const PtrUnitFilter IsGatheringGas;

    /// @see BWAPI::Unit::isGatheringMinerals
    extern const PtrUnitFilter IsGatheringMinerals;

    /// @see BWAPI::Unit::isHallucination
    extern const PtrUnitFilter IsHallucination;

    /// @see BWAPI::Unit::isHoldingPosition
    extern const PtrUnitFilter IsHoldingPosition;

    /// @see BWAPI::Unit::isIdle
    extern const PtrUnitFilter IsIdle;

    /// @see BWAPI::Unit::isInterruptible
    extern const PtrUnitFilter IsInterruptible;

    /// @see BWAPI::Unit::isInvincible
    extern const PtrUnitFilter IsInvincible;

    /// @see BWAPI::Unit::isIrradiated
    extern const PtrUnitFilter IsIrradiated;

    /// @see BWAPI::Unit::isLifted
    extern const PtrUnitFilter IsLifted;

    /// @see BWAPI::Unit::isLoaded
    extern const PtrUnitFilter IsLoaded;

    /// @see BWAPI::Unit::isLockedDown
    extern const PtrUnitFilter IsLockedDown;

    /// @see BWAPI::Unit::isMaelstrommed
    extern const PtrUnitFilter IsMaelstrommed;

    /// @see BWAPI::Unit::isMorphing
    extern const PtrUnitFilter IsMorphing;

    /// @see BWAPI::Unit::isMoving
    extern const PtrUnitFilter IsMoving;

    /// @see BWAPI::Unit::isParasited
    extern const PtrUnitFilter IsParasited;

    /// @see BWAPI::Unit::isPatrolling
    extern const PtrUnitFilter IsPatrolling;

    /// @see BWAPI::Unit::isPlagued
    extern const PtrUnitFilter IsPlagued;

    /// @see BWAPI::Unit::isRepairing
    extern const PtrUnitFilter IsRepairing;

    /// @see BWAPI::Unit::isResearching
    extern const PtrUnitFilter IsResearching;

    /// @see BWAPI::Unit::isSieged
    extern const PtrUnitFilter IsSieged;

    /// @see BWAPI::Unit::isStartingAttack
    extern const PtrUnitFilter IsStartingAttack;

    /// @see BWAPI::Unit::isStasised
    extern const PtrUnitFilter IsStasised;

    /// @see BWAPI::Unit::isStimmed
    extern const PtrUnitFilter IsStimmed;

    /// @see BWAPI::Unit::isStuck
    extern const PtrUnitFilter IsStuck;

    /// @see BWAPI::Unit::isTraining
    extern const PtrUnitFilter IsTraining;

    /// @see BWAPI::Unit::isUnderAttack
    extern const PtrUnitFilter IsUnderAttack;

    /// @see BWAPI::Unit::isUnderDarkSwarm
    extern const PtrUnitFilter IsUnderDarkSwarm;

    /// @see BWAPI::Unit::isUnderDisruptionWeb
    extern const PtrUnitFilter IsUnderDisruptionWeb;

    /// @see BWAPI::Unit::isUnderStorm
    extern const PtrUnitFilter IsUnderStorm;

    /// @see BWAPI::Unit::isPowered
    extern const PtrUnitFilter IsPowered;

    /// @see BWAPI::Unit::isVisible
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

