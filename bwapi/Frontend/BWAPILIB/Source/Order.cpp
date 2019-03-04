#include <string>
#include <BWAPI/Order.h>

namespace BWAPI
{
  template <>
  const std::string Type<Order, Orders::Enum::Unknown>::typeNames[Orders::Enum::MAX] =
  { 
    "Die", "Stop", "Guard", "PlayerGuard", "TurretGuard", "BunkerGuard", "Move", "ReaverStop", "Attack1", "Attack2",
    "AttackUnit", "AttackFixedRange", "AttackTile", "Hover", "AttackMove", "InfestedCommandCenter", "UnusedNothing", "UnusedPowerup", 
    "TowerGuard", "TowerAttack", "VultureMine", "StayInRange", "TurretAttack", "Nothing", "Unused_24", "DroneStartBuild", "DroneBuild", "CastInfestation",
    "MoveToInfest", "InfestingCommandCenter", "PlaceBuilding", "PlaceProtossBuilding", "CreateProtossBuilding", "ConstructingBuilding",
    "Repair", "MoveToRepair", "PlaceAddon", "BuildAddon", "Train", "RallyPointUnit", "RallyPointTile", 
    "ZergBirth", "ZergUnitMorph", "ZergBuildingMorph", "IncompleteBuilding", "IncompleteMorphing", "BuildNydusExit",
    "EnterNydusCanal", "IncompleteWarping", "Follow", "Carrier", "ReaverCarrierMove", "CarrierStop", "CarrierAttack", "CarrierMoveToAttack", "CarrierIgnore2",
    "CarrierFight", "CarrierHoldPosition", "Reaver", "ReaverAttack", "ReaverMoveToAttack", "ReaverFight", "ReaverHoldPosition", "TrainFighter", "InterceptorAttack", "ScarabAttack",
    "RechargeShieldsUnit", "RechargeShieldsBattery", "ShieldBattery", "InterceptorReturn", "DroneLand",
    "BuildingLand", "BuildingLiftOff", "DroneLiftOff", "LiftingOff", "ResearchTech", "Upgrade",
    "Larva", "SpawningLarva", "Harvest1", "Harvest2", "MoveToGas", "WaitForGas", "HarvestGas",
    "ReturnGas", "MoveToMinerals", "WaitForMinerals", "MiningMinerals", "Harvest3", "Harvest4",
    "ReturnMinerals", "Interrupted", "EnterTransport", "PickupIdle", "PickupTransport", "PickupBunker",
    "Pickup4", "PowerupIdle", "Sieging", "Unsieging", "WatchTarget", "InitCreepGrowth", "SpreadCreep",
    "StoppingCreepGrowth", "GuardianAspect", "ArchonWarp", "CompletingArchonSummon", "HoldPosition",
    "QueenHoldPosition", "Cloak", "Decloak", "Unload", "MoveUnload", "FireYamatoGun", "MoveToFireYamatoGun", "CastLockdown", "Burrowing",
    "Burrowed", "Unburrowing", "CastDarkSwarm", "CastParasite", "CastSpawnBroodlings", "CastEMPShockwave",
    "NukeWait", "NukeTrain", "NukeLaunch", "NukePaint", "NukeUnit", "CastNuclearStrike", "NukeTrack", "InitializeArbiter",
    "CloakNearbyUnits", "PlaceMine", "RightClickAction", "SuicideUnit", "SuicideLocation", "SuicideHoldPosition", "CastRecall", "Teleport",
    "CastScannerSweep", "Scanner", "CastDefensiveMatrix", "CastPsionicStorm", "CastIrradiate",
    "CastPlague", "CastConsume", "CastEnsnare", "CastStasisField", "CastHallucination", "Hallucination2",
    "ResetCollision", "ResetHarvestCollision", "Patrol", "CTFCOPInit", "CTFCOPStarted", "CTFCOP2", "ComputerAI", "AtkMoveEP",
    "HarassMove", "AIPatrol", "GuardPost", "RescuePassive", "Neutral", "ComputerReturn", "InitializePsiProvider",
    "SelfDestructing", "Critter", "HiddenGun", "OpenDoor", "CloseDoor", "HideTrap", "RevealTrap",
    "EnableDoodad", "DisableDoodad", "WarpIn", "Medic", "MedicHeal", "HealMove", "MedicHoldPosition", "MedicHealToIdle",
    "CastRestoration", "CastDisruptionWeb", "CastMindControl", "DarkArchonMeld", "CastFeedback",
    "CastOpticalFlare", "CastMaelstrom", "JunkYardDog", "Fatal", "None", "Unknown"
  };

  namespace OrderSet
  {
    using namespace Orders;
    const Order::set orderSet{ Die, Stop, Guard, PlayerGuard, TurretGuard, BunkerGuard, Move, AttackUnit,
      AttackTile, Hover, AttackMove, InfestedCommandCenter, UnusedNothing, UnusedPowerup,
      TowerGuard, VultureMine, Nothing, CastInfestation, InfestingCommandCenter,
      PlaceBuilding, CreateProtossBuilding, ConstructingBuilding, Repair, PlaceAddon, BuildAddon,
      Train, RallyPointUnit, RallyPointTile, ZergBirth, ZergUnitMorph, ZergBuildingMorph,
      IncompleteBuilding, BuildNydusExit, EnterNydusCanal, Follow, Carrier, ReaverCarrierMove,
      CarrierIgnore2, Reaver, TrainFighter, InterceptorAttack, ScarabAttack, RechargeShieldsUnit,
      RechargeShieldsBattery, ShieldBattery, InterceptorReturn, BuildingLand, BuildingLiftOff,
      DroneLiftOff, LiftingOff, ResearchTech, Upgrade, Larva, SpawningLarva, Harvest1, Harvest2,
      MoveToGas, WaitForGas, HarvestGas, ReturnGas, MoveToMinerals, WaitForMinerals, MiningMinerals,
      Harvest3, Harvest4, ReturnMinerals, Interrupted, EnterTransport, PickupIdle, PickupTransport,
      PickupBunker, Pickup4, PowerupIdle, Sieging, Unsieging, InitCreepGrowth, SpreadCreep,
      StoppingCreepGrowth, GuardianAspect, ArchonWarp, CompletingArchonSummon, HoldPosition,
      Cloak, Decloak, Unload, MoveUnload, FireYamatoGun, CastLockdown, Burrowing, Burrowed,
      Unburrowing, CastDarkSwarm, CastParasite, CastSpawnBroodlings, CastEMPShockwave, NukeWait,
      NukeTrain, NukeLaunch, NukePaint, NukeUnit, CastNuclearStrike, NukeTrack, CloakNearbyUnits,
      PlaceMine, RightClickAction, CastRecall, Teleport, CastScannerSweep, Scanner,
      CastDefensiveMatrix, CastPsionicStorm, CastIrradiate, CastPlague, CastConsume, CastEnsnare,
      CastStasisField, CastHallucination, Hallucination2, ResetCollision, Patrol, CTFCOPInit,
      CTFCOPStarted, CTFCOP2, ComputerAI, AtkMoveEP, HarassMove, AIPatrol, GuardPost, RescuePassive,
      Neutral, ComputerReturn, SelfDestructing, Critter, HiddenGun, OpenDoor, CloseDoor, HideTrap,
      RevealTrap, EnableDoodad, DisableDoodad, WarpIn, Medic, MedicHeal, HealMove, MedicHealToIdle,
      CastRestoration, CastDisruptionWeb, CastMindControl, DarkArchonMeld, CastFeedback, CastOpticalFlare,
      CastMaelstrom, JunkYardDog, Fatal, None, Unknown };
  }
  const Order::set& Orders::allOrders()
  {
    return OrderSet::orderSet;
  }
}
