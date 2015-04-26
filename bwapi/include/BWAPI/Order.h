#pragma once
#include <BWAPI/Type.h>

namespace BWAPI
{
  /// <summary>Namespace containing unit orders.</summary>
  /// @see Order
  namespace Orders
  {
    /// <summary>Enumeration of unit orders.</summary>
    /// @see Order
    namespace Enum
    {
      /// <summary>Enumeration of unit orders.</summary>
      /// @see Order
      enum Enum
      {
        Die,
        Stop,
        Guard,
        PlayerGuard,
        TurretGuard,
        BunkerGuard,
        Move,
        ReaverStop,
        Attack1,
        Attack2,
        AttackUnit,
        AttackFixedRange,
        AttackTile,
        Hover,
        AttackMove,
        InfestedCommandCenter,
        UnusedNothing,
        UnusedPowerup,
        TowerGuard,
        TowerAttack,
        VultureMine,
        StayInRange,
        TurretAttack,
        Nothing,
        Unused_24,
        DroneStartBuild,
        DroneBuild,
        CastInfestation,
        MoveToInfest,
        InfestingCommandCenter,
        PlaceBuilding,
        PlaceProtossBuilding,
        CreateProtossBuilding,
        ConstructingBuilding,
        Repair,
        MoveToRepair,
        PlaceAddon,
        BuildAddon,
        Train,
        RallyPointUnit,
        RallyPointTile,
        ZergBirth,
        ZergUnitMorph,
        ZergBuildingMorph,
        IncompleteBuilding,
        IncompleteMorphing,
        BuildNydusExit,
        EnterNydusCanal,
        IncompleteWarping,
        Follow,
        Carrier,
        ReaverCarrierMove,
        CarrierStop,
        CarrierAttack,
        CarrierMoveToAttack,
        CarrierIgnore2,
        CarrierFight,
        CarrierHoldPosition,
        Reaver,
        ReaverAttack,
        ReaverMoveToAttack,
        ReaverFight,
        ReaverHoldPosition,
        TrainFighter,
        InterceptorAttack,
        ScarabAttack,
        RechargeShieldsUnit,
        RechargeShieldsBattery,
        ShieldBattery,
        InterceptorReturn,
        DroneLand,
        BuildingLand,
        BuildingLiftOff,
        DroneLiftOff,
        LiftingOff,
        ResearchTech,
        Upgrade,
        Larva,
        SpawningLarva,
        Harvest1,
        Harvest2,
        MoveToGas,
        WaitForGas,
        HarvestGas,
        ReturnGas,
        MoveToMinerals,
        WaitForMinerals,
        MiningMinerals,
        Harvest3,
        Harvest4,
        ReturnMinerals,
        Interrupted,
        EnterTransport,
        PickupIdle,
        PickupTransport,
        PickupBunker,
        Pickup4,
        PowerupIdle,
        Sieging,
        Unsieging,
        WatchTarget,
        InitCreepGrowth,
        SpreadCreep,
        StoppingCreepGrowth,
        GuardianAspect,
        ArchonWarp,
        CompletingArchonSummon,
        HoldPosition,
        QueenHoldPosition,
        Cloak,
        Decloak,
        Unload,
        MoveUnload,
        FireYamatoGun,
        MoveToFireYamatoGun,
        CastLockdown,
        Burrowing,
        Burrowed,
        Unburrowing,
        CastDarkSwarm,
        CastParasite,
        CastSpawnBroodlings,
        CastEMPShockwave,
        NukeWait,
        NukeTrain,
        NukeLaunch,
        NukePaint,
        NukeUnit,
        CastNuclearStrike,
        NukeTrack,
        InitializeArbiter,
        CloakNearbyUnits,
        PlaceMine,
        RightClickAction,
        SuicideUnit,
        SuicideLocation,
        SuicideHoldPosition,
        CastRecall,
        Teleport,
        CastScannerSweep,
        Scanner,
        CastDefensiveMatrix,
        CastPsionicStorm,
        CastIrradiate,
        CastPlague,
        CastConsume,
        CastEnsnare,
        CastStasisField,
        CastHallucination,
        Hallucination2,
        ResetCollision,
        ResetHarvestCollision,
        Patrol,
        CTFCOPInit,
        CTFCOPStarted,
        CTFCOP2,
        ComputerAI,
        AtkMoveEP,
        HarassMove,
        AIPatrol,
        GuardPost,
        RescuePassive,
        Neutral,
        ComputerReturn,
        InitializePsiProvider,
        SelfDestructing,
        Critter,
        HiddenGun,
        OpenDoor,
        CloseDoor,
        HideTrap,
        RevealTrap,
        EnableDoodad,
        DisableDoodad,
        WarpIn,
        Medic,
        MedicHeal,
        HealMove,
        MedicHoldPosition,
        MedicHealToIdle,
        CastRestoration,
        CastDisruptionWeb,
        CastMindControl,
        DarkArchonMeld,
        CastFeedback,
        CastOpticalFlare,
        CastMaelstrom,
        JunkYardDog,
        Fatal,
        None,
        Unknown,
        MAX
      };
    }
  }
  /// <summary>An Order (Order type) represents a Unit's current action and can be retrieved with
  /// UnitInterface::getOrder.</summary> It can also be used to identify the current state of the
  /// unit during command execution (gathering minerals can consist of Orders::MoveToMinerals,
  /// Orders::WaitForMinerals, Orders::MiningMinerals, etc.).
  ///
  /// @see UnitInterface::getOrder, Orders
  /// @ingroup TypeClasses
  class Order : public Type<Order, Orders::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      Order(int id = Orders::Enum::None);
  };

  /// @ingroup Types
  namespace Orders
  {
    /// <summary>Retrieves the set of all defined game orders.</summary>
    ///
    /// @returns Set of all Order types.
    const Order::set& allOrders();

    extern const Order Die;
    extern const Order Stop;
    extern const Order Guard;
    extern const Order PlayerGuard;
    extern const Order TurretGuard;
    extern const Order BunkerGuard;
    extern const Order Move;
    extern const Order AttackUnit;
    extern const Order AttackTile;
    extern const Order Hover;
    extern const Order AttackMove;
    extern const Order InfestedCommandCenter;
    extern const Order UnusedNothing;
    extern const Order UnusedPowerup;
    extern const Order TowerGuard;
    extern const Order VultureMine;
    extern const Order Nothing;
    extern const Order CastInfestation;
    extern const Order InfestingCommandCenter;
    extern const Order PlaceBuilding;
    extern const Order CreateProtossBuilding;
    extern const Order ConstructingBuilding;
    extern const Order Repair;
    extern const Order PlaceAddon;
    extern const Order BuildAddon;
    extern const Order Train;
    extern const Order RallyPointUnit;
    extern const Order RallyPointTile;
    extern const Order ZergBirth;
    extern const Order ZergUnitMorph;
    extern const Order ZergBuildingMorph;
    extern const Order IncompleteBuilding;
    extern const Order BuildNydusExit;
    extern const Order EnterNydusCanal;
    extern const Order Follow;
    extern const Order Carrier;
    extern const Order ReaverCarrierMove;
    extern const Order CarrierIgnore2;
    extern const Order Reaver;
    extern const Order TrainFighter;
    extern const Order InterceptorAttack;
    extern const Order ScarabAttack;
    extern const Order RechargeShieldsUnit;
    extern const Order RechargeShieldsBattery;
    extern const Order ShieldBattery;
    extern const Order InterceptorReturn;
    extern const Order BuildingLand;
    extern const Order BuildingLiftOff;
    extern const Order DroneLiftOff;
    extern const Order LiftingOff;
    extern const Order ResearchTech;
    extern const Order Upgrade;
    extern const Order Larva;
    extern const Order SpawningLarva;
    extern const Order Harvest1;
    extern const Order Harvest2;
    extern const Order MoveToGas;
    extern const Order WaitForGas;
    extern const Order HarvestGas;
    extern const Order ReturnGas;
    extern const Order MoveToMinerals;
    extern const Order WaitForMinerals;
    extern const Order MiningMinerals;
    extern const Order Harvest3;
    extern const Order Harvest4;
    extern const Order ReturnMinerals;
    extern const Order Interrupted;
    extern const Order EnterTransport;
    extern const Order PickupIdle;
    extern const Order PickupTransport;
    extern const Order PickupBunker;
    extern const Order Pickup4;
    extern const Order PowerupIdle;
    extern const Order Sieging;
    extern const Order Unsieging;
    extern const Order InitCreepGrowth;
    extern const Order SpreadCreep;
    extern const Order StoppingCreepGrowth;
    extern const Order GuardianAspect;
    extern const Order ArchonWarp;
    extern const Order CompletingArchonSummon;
    extern const Order HoldPosition;
    extern const Order Cloak;
    extern const Order Decloak;
    extern const Order Unload;
    extern const Order MoveUnload;
    extern const Order FireYamatoGun;
    extern const Order CastLockdown;
    extern const Order Burrowing;
    extern const Order Burrowed;
    extern const Order Unburrowing;
    extern const Order CastDarkSwarm;
    extern const Order CastParasite;
    extern const Order CastSpawnBroodlings;
    extern const Order CastEMPShockwave;
    extern const Order NukeWait;
    extern const Order NukeTrain;
    extern const Order NukeLaunch;
    extern const Order NukePaint;
    extern const Order NukeUnit;
    extern const Order CastNuclearStrike;
    extern const Order NukeTrack;
    extern const Order CloakNearbyUnits;
    extern const Order PlaceMine;
    extern const Order RightClickAction;
    extern const Order CastRecall;
    extern const Order Teleport;
    extern const Order CastScannerSweep;
    extern const Order Scanner;
    extern const Order CastDefensiveMatrix;
    extern const Order CastPsionicStorm;
    extern const Order CastIrradiate;
    extern const Order CastPlague;
    extern const Order CastConsume;
    extern const Order CastEnsnare;
    extern const Order CastStasisField;
    extern const Order CastHallucination;
    extern const Order Hallucination2;
    extern const Order ResetCollision;
    extern const Order Patrol;
    extern const Order CTFCOPInit;
    extern const Order CTFCOPStarted;
    extern const Order CTFCOP2;
    extern const Order ComputerAI;
    extern const Order AtkMoveEP;
    extern const Order HarassMove;
    extern const Order AIPatrol;
    extern const Order GuardPost;
    extern const Order RescuePassive;
    extern const Order Neutral;
    extern const Order ComputerReturn;
    extern const Order SelfDestructing;
    extern const Order Critter;
    extern const Order HiddenGun;
    extern const Order OpenDoor;
    extern const Order CloseDoor;
    extern const Order HideTrap;
    extern const Order RevealTrap;
    extern const Order EnableDoodad;
    extern const Order DisableDoodad;
    extern const Order WarpIn;
    extern const Order Medic;
    extern const Order MedicHeal;
    extern const Order HealMove;
    extern const Order MedicHealToIdle;
    extern const Order CastRestoration;
    extern const Order CastDisruptionWeb;
    extern const Order CastMindControl;
    extern const Order DarkArchonMeld;
    extern const Order CastFeedback;
    extern const Order CastOpticalFlare;
    extern const Order CastMaelstrom;
    extern const Order JunkYardDog;
    extern const Order Fatal;
    extern const Order None;
    extern const Order Unknown;
  };

  static_assert(sizeof(Order) == sizeof(int), "Expected type to resolve to primitive size.");
}
