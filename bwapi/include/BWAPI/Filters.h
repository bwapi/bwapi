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
    // presets
    extern const PtrUnitFilter IsTransport;

    extern const PtrUnitFilter CanProduce;
    extern const PtrUnitFilter CanAttack;
    extern const PtrUnitFilter CanMove;
    extern const PtrUnitFilter IsFlyer;
    extern const PtrUnitFilter IsFlying;
    extern const PtrUnitFilter RegeneratesHP;
    extern const PtrUnitFilter IsSpellcaster;
    extern const PtrUnitFilter HasPermanentCloak;
    extern const PtrUnitFilter IsOrganic;
    extern const PtrUnitFilter IsMechanical;
    extern const PtrUnitFilter IsRobotic;
    extern const PtrUnitFilter IsDetector;
    extern const PtrUnitFilter IsResourceContainer;
    extern const PtrUnitFilter IsResourceDepot;
    extern const PtrUnitFilter IsRefinery;
    extern const PtrUnitFilter IsWorker;
    extern const PtrUnitFilter RequiresPsi;
    extern const PtrUnitFilter RequiresCreep;
    extern const PtrUnitFilter IsBurrowable;
    extern const PtrUnitFilter IsCloakable;
    extern const PtrUnitFilter IsBuilding;
    extern const PtrUnitFilter IsAddon;
    extern const PtrUnitFilter IsFlyingBuilding;
    extern const PtrUnitFilter IsNeutral;
    extern const PtrUnitFilter IsHero;
    extern const PtrUnitFilter IsPowerup;
    extern const PtrUnitFilter IsBeacon;
    extern const PtrUnitFilter IsFlagBeacon;
    extern const PtrUnitFilter IsSpecialBuilding;
    extern const PtrUnitFilter IsSpell;
    extern const PtrUnitFilter ProducesLarva;
    extern const PtrUnitFilter IsMineralField;
    extern const PtrUnitFilter IsCritter;
    extern const PtrUnitFilter CanBuildAddon;
  
    extern const PtrIntCompareUnitFilter HP;
    extern const PtrIntCompareUnitFilter MaxHP;
    extern const PtrIntCompareUnitFilter HP_Percent;
  
    extern const PtrIntCompareUnitFilter Shields;
    extern const PtrIntCompareUnitFilter MaxShields;
    extern const PtrIntCompareUnitFilter Shields_Percent;
  
    extern const PtrIntCompareUnitFilter Energy;
    extern const PtrIntCompareUnitFilter MaxEnergy;
    extern const PtrIntCompareUnitFilter Energy_Percent;

    extern const PtrIntCompareUnitFilter Armor;
    extern const CompareFilter<Unit ,UpgradeType,UpgradeType (*)(Unit )> ArmorUpgrade;

    extern const PtrIntCompareUnitFilter MineralPrice;
    extern const PtrIntCompareUnitFilter GasPrice;
    extern const PtrIntCompareUnitFilter BuildTime;

    extern const PtrIntCompareUnitFilter SupplyRequired;
    extern const PtrIntCompareUnitFilter SupplyProvided;

    extern const PtrIntCompareUnitFilter SpaceRequired;
    extern const PtrIntCompareUnitFilter SpaceRemaining;
    extern const PtrIntCompareUnitFilter SpaceProvided;

    extern const PtrIntCompareUnitFilter BuildScore;
    extern const PtrIntCompareUnitFilter DestroyScore;

    extern const CompareFilter<Unit ,double,double (*)(Unit )> TopSpeed;
    extern const PtrIntCompareUnitFilter SightRange;
    extern const PtrIntCompareUnitFilter WeaponCooldown;
    extern const CompareFilter<Unit ,UnitSizeType,UnitSizeType (*)(Unit )> SizeType;

    extern const CompareFilter<Unit ,WeaponType,WeaponType (*)(Unit )> GroundWeapon;
    extern const CompareFilter<Unit ,WeaponType,WeaponType (*)(Unit )> AirWeapon;

    extern const CompareFilter<Unit ,UnitType,UnitType (*)(Unit )> GetType;
    extern const CompareFilter<Unit ,Race,Race (*)(Unit )> GetRace;
    extern const CompareFilter<Unit ,Player,Player (*)(Unit )> GetPlayer;
  
    extern const PtrIntCompareUnitFilter Resources;
    extern const PtrIntCompareUnitFilter ResourceGroup;
    extern const PtrIntCompareUnitFilter AcidSporeCount;
    extern const PtrIntCompareUnitFilter InterceptorCount;
    extern const PtrIntCompareUnitFilter ScarabCount;
    extern const PtrIntCompareUnitFilter SpiderMineCount;
    extern const PtrIntCompareUnitFilter MaxWeaponCooldown;
    extern const PtrIntCompareUnitFilter SpellCooldown;

    extern const PtrIntCompareUnitFilter DefenseMatrixPoints;
    extern const PtrIntCompareUnitFilter DefenseMatrixTime;
    extern const PtrIntCompareUnitFilter EnsnareTime;
    extern const PtrIntCompareUnitFilter IrradiateTime;
    extern const PtrIntCompareUnitFilter LockdownTime;
    extern const PtrIntCompareUnitFilter MaelstromTime;
    extern const PtrIntCompareUnitFilter OrderTime;
    extern const PtrIntCompareUnitFilter PlagueTimer;
    extern const PtrIntCompareUnitFilter RemoveTime;
    extern const PtrIntCompareUnitFilter StasisTime;
    extern const PtrIntCompareUnitFilter StimTime;
    extern const CompareFilter<Unit ,UnitType,UnitType (*)(Unit )> BuildType;
    extern const PtrIntCompareUnitFilter RemainingBuildTime;
    extern const PtrIntCompareUnitFilter RemainingTrainTime;
    extern const CompareFilter<Unit ,Unit ,Unit (*)(Unit )> Target;
    extern const CompareFilter<Unit ,Order,Order (*)(Unit )> CurrentOrder;
    extern const CompareFilter<Unit ,Order,Order (*)(Unit )> SecondaryOrder;
    extern const CompareFilter<Unit ,Unit ,Unit (*)(Unit )> OrderTarget;
    extern const PtrIntCompareUnitFilter GetLeft;
    extern const PtrIntCompareUnitFilter GetTop;
    extern const PtrIntCompareUnitFilter GetRight;
    extern const PtrIntCompareUnitFilter GetBottom;

    extern const PtrUnitFilter Exists;
    extern const PtrUnitFilter IsAttacking;
    extern const PtrUnitFilter IsBeingConstructed;
    extern const PtrUnitFilter IsBeingGathered;
    extern const PtrUnitFilter IsBeingHealed;
    extern const PtrUnitFilter IsBlind;
    extern const PtrUnitFilter IsBraking;
    extern const PtrUnitFilter IsBurrowed;
    extern const PtrUnitFilter IsCarryingGas;
    extern const PtrUnitFilter IsCarryingMinerals;
    extern const PtrUnitFilter IsCarryingSomething;
    extern const PtrUnitFilter IsCloaked;
    extern const PtrUnitFilter IsCompleted;
    extern const PtrUnitFilter IsConstructing;
    extern const PtrUnitFilter IsDefenseMatrixed;
    extern const PtrUnitFilter IsDetected;
    extern const PtrUnitFilter IsEnsnared;
    extern const PtrUnitFilter IsFollowing;
    extern const PtrUnitFilter IsGatheringGas;
    extern const PtrUnitFilter IsGatheringMinerals;
    extern const PtrUnitFilter IsHallucination;
    extern const PtrUnitFilter IsHoldingPosition;
    extern const PtrUnitFilter IsIdle;
    extern const PtrUnitFilter IsInterruptible;
    extern const PtrUnitFilter IsInvincible;
    extern const PtrUnitFilter IsIrradiated;
    extern const PtrUnitFilter IsLifted;
    extern const PtrUnitFilter IsLoaded;
    extern const PtrUnitFilter IsLockedDown;
    extern const PtrUnitFilter IsMaelstrommed;
    extern const PtrUnitFilter IsMorphing;
    extern const PtrUnitFilter IsMoving;
    extern const PtrUnitFilter IsParasited;
    extern const PtrUnitFilter IsPatrolling;
    extern const PtrUnitFilter IsPlagued;
    extern const PtrUnitFilter IsRepairing;
    extern const PtrUnitFilter IsResearching;
    extern const PtrUnitFilter IsSieged;
    extern const PtrUnitFilter IsStartingAttack;
    extern const PtrUnitFilter IsStasised;
    extern const PtrUnitFilter IsStimmed;
    extern const PtrUnitFilter IsStuck;
    extern const PtrUnitFilter IsTraining;
    extern const PtrUnitFilter IsUnderAttack;
    extern const PtrUnitFilter IsUnderDarkSwarm;
    extern const PtrUnitFilter IsUnderDisruptionWeb;
    extern const PtrUnitFilter IsUnderStorm;
    extern const PtrUnitFilter IsUnpowered;
    extern const PtrUnitFilter IsVisible;
  
    extern const PtrUnitFilter IsEnemy;
    extern const PtrUnitFilter IsAlly;
    extern const PtrUnitFilter IsOwned;
  }
}

