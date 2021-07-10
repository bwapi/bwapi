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
  /// Unit::getOrder.</summary> It can also be used to identify the current state of the
  /// unit during command execution (gathering minerals can consist of Orders::MoveToMinerals,
  /// Orders::WaitForMinerals, Orders::MiningMinerals, etc.).
  ///
  /// @see Unit::getOrder, Orders
  /// @ingroup TypeClasses
  class Order : public Type<Order, Orders::Enum::Unknown>
  {
    public:
      /// @copydoc Type::Type(int)
      constexpr explicit Order(int id = Orders::Enum::None) : Type(id) {}
  };

  /// @ingroup Types
  namespace Orders
  {
    /// <summary>Retrieves the set of all defined game orders.</summary>
    ///
    /// @returns Set of all Order types.
    const Order::set& allOrders();

    constexpr Order Die{Enum::Die};
    constexpr Order Stop{Enum::Stop};
    constexpr Order Guard{Enum::Guard};
    constexpr Order PlayerGuard{Enum::PlayerGuard};
    constexpr Order TurretGuard{Enum::TurretGuard};
    constexpr Order BunkerGuard{Enum::BunkerGuard};
    constexpr Order Move{Enum::Move};
    constexpr Order AttackUnit{Enum::AttackUnit};
    constexpr Order AttackTile{Enum::AttackTile};
    constexpr Order Hover{Enum::Hover};
    constexpr Order AttackMove{Enum::AttackMove};
    constexpr Order InfestedCommandCenter{Enum::InfestedCommandCenter};
    constexpr Order UnusedNothing{Enum::UnusedNothing};
    constexpr Order UnusedPowerup{Enum::UnusedPowerup};
    constexpr Order TowerGuard{Enum::TowerGuard};
    constexpr Order VultureMine{Enum::VultureMine};
    constexpr Order Nothing{Enum::Nothing};
    constexpr Order CastInfestation{Enum::CastInfestation};
    constexpr Order InfestingCommandCenter{Enum::InfestingCommandCenter};
    constexpr Order PlaceBuilding{Enum::PlaceBuilding};
    constexpr Order CreateProtossBuilding{Enum::CreateProtossBuilding};
    constexpr Order ConstructingBuilding{Enum::ConstructingBuilding};
    constexpr Order Repair{Enum::Repair};
    constexpr Order PlaceAddon{Enum::PlaceAddon};
    constexpr Order BuildAddon{Enum::BuildAddon};
    constexpr Order Train{Enum::Train};
    constexpr Order RallyPointUnit{Enum::RallyPointUnit};
    constexpr Order RallyPointTile{Enum::RallyPointTile};
    constexpr Order ZergBirth{Enum::ZergBirth};
    constexpr Order ZergUnitMorph{Enum::ZergUnitMorph};
    constexpr Order ZergBuildingMorph{Enum::ZergBuildingMorph};
    constexpr Order IncompleteBuilding{Enum::IncompleteBuilding};
    constexpr Order BuildNydusExit{Enum::BuildNydusExit};
    constexpr Order EnterNydusCanal{Enum::EnterNydusCanal};
    constexpr Order Follow{Enum::Follow};
    constexpr Order Carrier{Enum::Carrier};
    constexpr Order ReaverCarrierMove{Enum::ReaverCarrierMove};
    constexpr Order CarrierIgnore2{Enum::CarrierIgnore2};
    constexpr Order Reaver{Enum::Reaver};
    constexpr Order TrainFighter{Enum::TrainFighter};
    constexpr Order InterceptorAttack{Enum::InterceptorAttack};
    constexpr Order ScarabAttack{Enum::ScarabAttack};
    constexpr Order RechargeShieldsUnit{Enum::RechargeShieldsUnit};
    constexpr Order RechargeShieldsBattery{Enum::RechargeShieldsBattery};
    constexpr Order ShieldBattery{Enum::ShieldBattery};
    constexpr Order InterceptorReturn{Enum::InterceptorReturn};
    constexpr Order BuildingLand{Enum::BuildingLand};
    constexpr Order BuildingLiftOff{Enum::BuildingLiftOff};
    constexpr Order DroneLiftOff{Enum::DroneLiftOff};
    constexpr Order LiftingOff{Enum::LiftingOff};
    constexpr Order ResearchTech{Enum::ResearchTech};
    constexpr Order Upgrade{Enum::Upgrade};
    constexpr Order Larva{Enum::Larva};
    constexpr Order SpawningLarva{Enum::SpawningLarva};
    constexpr Order Harvest1{Enum::Harvest1};
    constexpr Order Harvest2{Enum::Harvest2};
    constexpr Order MoveToGas{Enum::MoveToGas};
    constexpr Order WaitForGas{Enum::WaitForGas};
    constexpr Order HarvestGas{Enum::HarvestGas};
    constexpr Order ReturnGas{Enum::ReturnGas};
    constexpr Order MoveToMinerals{Enum::MoveToMinerals};
    constexpr Order WaitForMinerals{Enum::WaitForMinerals};
    constexpr Order MiningMinerals{Enum::MiningMinerals};
    constexpr Order Harvest3{Enum::Harvest3};
    constexpr Order Harvest4{Enum::Harvest4};
    constexpr Order ReturnMinerals{Enum::ReturnMinerals};
    constexpr Order Interrupted{Enum::Interrupted};
    constexpr Order EnterTransport{Enum::EnterTransport};
    constexpr Order PickupIdle{Enum::PickupIdle};
    constexpr Order PickupTransport{Enum::PickupTransport};
    constexpr Order PickupBunker{Enum::PickupBunker};
    constexpr Order Pickup4{Enum::Pickup4};
    constexpr Order PowerupIdle{Enum::PowerupIdle};
    constexpr Order Sieging{Enum::Sieging};
    constexpr Order Unsieging{Enum::Unsieging};
    constexpr Order InitCreepGrowth{Enum::InitCreepGrowth};
    constexpr Order SpreadCreep{Enum::SpreadCreep};
    constexpr Order StoppingCreepGrowth{Enum::StoppingCreepGrowth};
    constexpr Order GuardianAspect{Enum::GuardianAspect};
    constexpr Order ArchonWarp{Enum::ArchonWarp};
    constexpr Order CompletingArchonSummon{Enum::CompletingArchonSummon};
    constexpr Order HoldPosition{Enum::HoldPosition};
    constexpr Order Cloak{Enum::Cloak};
    constexpr Order Decloak{Enum::Decloak};
    constexpr Order Unload{Enum::Unload};
    constexpr Order MoveUnload{Enum::MoveUnload};
    constexpr Order FireYamatoGun{Enum::FireYamatoGun};
    constexpr Order CastLockdown{Enum::CastLockdown};
    constexpr Order Burrowing{Enum::Burrowing};
    constexpr Order Burrowed{Enum::Burrowed};
    constexpr Order Unburrowing{Enum::Unburrowing};
    constexpr Order CastDarkSwarm{Enum::CastDarkSwarm};
    constexpr Order CastParasite{Enum::CastParasite};
    constexpr Order CastSpawnBroodlings{Enum::CastSpawnBroodlings};
    constexpr Order CastEMPShockwave{Enum::CastEMPShockwave};
    constexpr Order NukeWait{Enum::NukeWait};
    constexpr Order NukeTrain{Enum::NukeTrain};
    constexpr Order NukeLaunch{Enum::NukeLaunch};
    constexpr Order NukePaint{Enum::NukePaint};
    constexpr Order NukeUnit{Enum::NukeUnit};
    constexpr Order CastNuclearStrike{Enum::CastNuclearStrike};
    constexpr Order NukeTrack{Enum::NukeTrack};
    constexpr Order CloakNearbyUnits{Enum::CloakNearbyUnits};
    constexpr Order PlaceMine{Enum::PlaceMine};
    constexpr Order RightClickAction{Enum::RightClickAction};
    constexpr Order CastRecall{Enum::CastRecall};
    constexpr Order Teleport{Enum::Teleport};
    constexpr Order CastScannerSweep{Enum::CastScannerSweep};
    constexpr Order Scanner{Enum::Scanner};
    constexpr Order CastDefensiveMatrix{Enum::CastDefensiveMatrix};
    constexpr Order CastPsionicStorm{Enum::CastPsionicStorm};
    constexpr Order CastIrradiate{Enum::CastIrradiate};
    constexpr Order CastPlague{Enum::CastPlague};
    constexpr Order CastConsume{Enum::CastConsume};
    constexpr Order CastEnsnare{Enum::CastEnsnare};
    constexpr Order CastStasisField{Enum::CastStasisField};
    constexpr Order CastHallucination{Enum::CastHallucination};
    constexpr Order Hallucination2{Enum::Hallucination2};
    constexpr Order ResetCollision{Enum::ResetCollision};
    constexpr Order Patrol{Enum::Patrol};
    constexpr Order CTFCOPInit{Enum::CTFCOPInit};
    constexpr Order CTFCOPStarted{Enum::CTFCOPStarted};
    constexpr Order CTFCOP2{Enum::CTFCOP2};
    constexpr Order ComputerAI{Enum::ComputerAI};
    constexpr Order AtkMoveEP{Enum::AtkMoveEP};
    constexpr Order HarassMove{Enum::HarassMove};
    constexpr Order AIPatrol{Enum::AIPatrol};
    constexpr Order GuardPost{Enum::GuardPost};
    constexpr Order RescuePassive{Enum::RescuePassive};
    constexpr Order Neutral{Enum::Neutral};
    constexpr Order ComputerReturn{Enum::ComputerReturn};
    constexpr Order SelfDestructing{Enum::SelfDestructing};
    constexpr Order Critter{Enum::Critter};
    constexpr Order HiddenGun{Enum::HiddenGun};
    constexpr Order OpenDoor{Enum::OpenDoor};
    constexpr Order CloseDoor{Enum::CloseDoor};
    constexpr Order HideTrap{Enum::HideTrap};
    constexpr Order RevealTrap{Enum::RevealTrap};
    constexpr Order EnableDoodad{Enum::EnableDoodad};
    constexpr Order DisableDoodad{Enum::DisableDoodad};
    constexpr Order WarpIn{Enum::WarpIn};
    constexpr Order Medic{Enum::Medic};
    constexpr Order MedicHeal{Enum::MedicHeal};
    constexpr Order HealMove{Enum::HealMove};
    constexpr Order MedicHealToIdle{Enum::MedicHealToIdle};
    constexpr Order CastRestoration{Enum::CastRestoration};
    constexpr Order CastDisruptionWeb{Enum::CastDisruptionWeb};
    constexpr Order CastMindControl{Enum::CastMindControl};
    constexpr Order DarkArchonMeld{Enum::DarkArchonMeld};
    constexpr Order CastFeedback{Enum::CastFeedback};
    constexpr Order CastOpticalFlare{Enum::CastOpticalFlare};
    constexpr Order CastMaelstrom{Enum::CastMaelstrom};
    constexpr Order JunkYardDog{Enum::JunkYardDog};
    constexpr Order Fatal{Enum::Fatal};
    constexpr Order None{Enum::None};
    constexpr Order Unknown{Enum::Unknown};
  }

  static_assert(sizeof(Order) == sizeof(int), "Expected type to resolve to primitive size.");
} // namespace BWAPI

namespace std {
  template<>
  struct hash<BWAPI::Order> {
    auto operator()(BWAPI::Order id) const {
      return BWAPI::Order::Hash{}(id);
    }
  };
} // namespace std
