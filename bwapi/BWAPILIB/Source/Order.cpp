#include <string>
#include <BWAPI/Order.h>

#include "Common.h"

#include "../../Debug.h"

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
    using namespace Orders::Enum;
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
  namespace Orders
  {
    BWAPI_TYPEDEF(Order,Die);
    BWAPI_TYPEDEF(Order,Stop);
    BWAPI_TYPEDEF(Order,Guard);
    BWAPI_TYPEDEF(Order,PlayerGuard);
    BWAPI_TYPEDEF(Order,TurretGuard);
    BWAPI_TYPEDEF(Order,BunkerGuard);
    BWAPI_TYPEDEF(Order,Move);
    BWAPI_TYPEDEF(Order,AttackUnit);
    BWAPI_TYPEDEF(Order,AttackTile);
    BWAPI_TYPEDEF(Order,Hover);
    BWAPI_TYPEDEF(Order,AttackMove);
    BWAPI_TYPEDEF(Order,InfestedCommandCenter);
    BWAPI_TYPEDEF(Order,UnusedNothing);
    BWAPI_TYPEDEF(Order,UnusedPowerup);
    BWAPI_TYPEDEF(Order,TowerGuard);
    BWAPI_TYPEDEF(Order,VultureMine);
    BWAPI_TYPEDEF(Order,Nothing);
    BWAPI_TYPEDEF(Order,CastInfestation);
    BWAPI_TYPEDEF(Order,InfestingCommandCenter);
    BWAPI_TYPEDEF(Order,PlaceBuilding);
    BWAPI_TYPEDEF(Order,CreateProtossBuilding);
    BWAPI_TYPEDEF(Order,ConstructingBuilding);
    BWAPI_TYPEDEF(Order,Repair);
    BWAPI_TYPEDEF(Order,PlaceAddon);
    BWAPI_TYPEDEF(Order,BuildAddon);
    BWAPI_TYPEDEF(Order,Train);
    BWAPI_TYPEDEF(Order,RallyPointUnit);
    BWAPI_TYPEDEF(Order,RallyPointTile);
    BWAPI_TYPEDEF(Order,ZergBirth);
    BWAPI_TYPEDEF(Order,ZergUnitMorph);
    BWAPI_TYPEDEF(Order,ZergBuildingMorph);
    BWAPI_TYPEDEF(Order,IncompleteBuilding);
    BWAPI_TYPEDEF(Order,BuildNydusExit);
    BWAPI_TYPEDEF(Order,EnterNydusCanal);
    BWAPI_TYPEDEF(Order,Follow);
    BWAPI_TYPEDEF(Order,Carrier);
    BWAPI_TYPEDEF(Order,ReaverCarrierMove);
    BWAPI_TYPEDEF(Order,CarrierIgnore2);
    BWAPI_TYPEDEF(Order,Reaver);
    BWAPI_TYPEDEF(Order,TrainFighter);
    BWAPI_TYPEDEF(Order,InterceptorAttack);
    BWAPI_TYPEDEF(Order,ScarabAttack);
    BWAPI_TYPEDEF(Order,RechargeShieldsUnit);
    BWAPI_TYPEDEF(Order,RechargeShieldsBattery);
    BWAPI_TYPEDEF(Order,ShieldBattery);
    BWAPI_TYPEDEF(Order,InterceptorReturn);
    BWAPI_TYPEDEF(Order,BuildingLand);
    BWAPI_TYPEDEF(Order,BuildingLiftOff);
    BWAPI_TYPEDEF(Order,DroneLiftOff);
    BWAPI_TYPEDEF(Order,LiftingOff);
    BWAPI_TYPEDEF(Order,ResearchTech);
    BWAPI_TYPEDEF(Order,Upgrade);
    BWAPI_TYPEDEF(Order,Larva);
    BWAPI_TYPEDEF(Order,SpawningLarva);
    BWAPI_TYPEDEF(Order,Harvest1);
    BWAPI_TYPEDEF(Order,Harvest2);
    BWAPI_TYPEDEF(Order,MoveToGas);
    BWAPI_TYPEDEF(Order,WaitForGas);
    BWAPI_TYPEDEF(Order,HarvestGas);
    BWAPI_TYPEDEF(Order,ReturnGas);
    BWAPI_TYPEDEF(Order,MoveToMinerals);
    BWAPI_TYPEDEF(Order,WaitForMinerals);
    BWAPI_TYPEDEF(Order,MiningMinerals);
    BWAPI_TYPEDEF(Order,Harvest3);
    BWAPI_TYPEDEF(Order,Harvest4);
    BWAPI_TYPEDEF(Order,ReturnMinerals);
    BWAPI_TYPEDEF(Order,Interrupted);
    BWAPI_TYPEDEF(Order,EnterTransport);
    BWAPI_TYPEDEF(Order,PickupIdle);
    BWAPI_TYPEDEF(Order,PickupTransport);
    BWAPI_TYPEDEF(Order,PickupBunker);
    BWAPI_TYPEDEF(Order,Pickup4);
    BWAPI_TYPEDEF(Order,PowerupIdle);
    BWAPI_TYPEDEF(Order,Sieging);
    BWAPI_TYPEDEF(Order,Unsieging);
    BWAPI_TYPEDEF(Order,InitCreepGrowth);
    BWAPI_TYPEDEF(Order,SpreadCreep);
    BWAPI_TYPEDEF(Order,StoppingCreepGrowth);
    BWAPI_TYPEDEF(Order,GuardianAspect);
    BWAPI_TYPEDEF(Order,ArchonWarp);
    BWAPI_TYPEDEF(Order,CompletingArchonSummon);
    BWAPI_TYPEDEF(Order,HoldPosition);
    BWAPI_TYPEDEF(Order,Cloak);
    BWAPI_TYPEDEF(Order,Decloak);
    BWAPI_TYPEDEF(Order,Unload);
    BWAPI_TYPEDEF(Order,MoveUnload);
    BWAPI_TYPEDEF(Order,FireYamatoGun);
    BWAPI_TYPEDEF(Order,CastLockdown);
    BWAPI_TYPEDEF(Order,Burrowing);
    BWAPI_TYPEDEF(Order,Burrowed);
    BWAPI_TYPEDEF(Order,Unburrowing);
    BWAPI_TYPEDEF(Order,CastDarkSwarm);
    BWAPI_TYPEDEF(Order,CastParasite);
    BWAPI_TYPEDEF(Order,CastSpawnBroodlings);
    BWAPI_TYPEDEF(Order,CastEMPShockwave);
    BWAPI_TYPEDEF(Order,NukeWait);
    BWAPI_TYPEDEF(Order,NukeTrain);
    BWAPI_TYPEDEF(Order,NukeLaunch);
    BWAPI_TYPEDEF(Order,NukePaint);
    BWAPI_TYPEDEF(Order,NukeUnit);
    BWAPI_TYPEDEF(Order,CastNuclearStrike);
    BWAPI_TYPEDEF(Order,NukeTrack);
    BWAPI_TYPEDEF(Order,CloakNearbyUnits);
    BWAPI_TYPEDEF(Order,PlaceMine);
    BWAPI_TYPEDEF(Order,RightClickAction);
    BWAPI_TYPEDEF(Order,CastRecall);
    BWAPI_TYPEDEF(Order,Teleport);
    BWAPI_TYPEDEF(Order,CastScannerSweep);
    BWAPI_TYPEDEF(Order,Scanner);
    BWAPI_TYPEDEF(Order,CastDefensiveMatrix);
    BWAPI_TYPEDEF(Order,CastPsionicStorm);
    BWAPI_TYPEDEF(Order,CastIrradiate);
    BWAPI_TYPEDEF(Order,CastPlague);
    BWAPI_TYPEDEF(Order,CastConsume);
    BWAPI_TYPEDEF(Order,CastEnsnare);
    BWAPI_TYPEDEF(Order,CastStasisField);
    BWAPI_TYPEDEF(Order,CastHallucination);
    BWAPI_TYPEDEF(Order,Hallucination2);
    BWAPI_TYPEDEF(Order,ResetCollision);
    BWAPI_TYPEDEF(Order,Patrol);
    BWAPI_TYPEDEF(Order,CTFCOPInit);
    BWAPI_TYPEDEF(Order,CTFCOPStarted);
    BWAPI_TYPEDEF(Order,CTFCOP2);
    BWAPI_TYPEDEF(Order,ComputerAI);
    BWAPI_TYPEDEF(Order,AtkMoveEP);
    BWAPI_TYPEDEF(Order,HarassMove);
    BWAPI_TYPEDEF(Order,AIPatrol);
    BWAPI_TYPEDEF(Order,GuardPost);
    BWAPI_TYPEDEF(Order,RescuePassive);
    BWAPI_TYPEDEF(Order,Neutral);
    BWAPI_TYPEDEF(Order,ComputerReturn);
    BWAPI_TYPEDEF(Order,SelfDestructing);
    BWAPI_TYPEDEF(Order,Critter);
    BWAPI_TYPEDEF(Order,HiddenGun);
    BWAPI_TYPEDEF(Order,OpenDoor);
    BWAPI_TYPEDEF(Order,CloseDoor);
    BWAPI_TYPEDEF(Order,HideTrap);
    BWAPI_TYPEDEF(Order,RevealTrap);
    BWAPI_TYPEDEF(Order,EnableDoodad);
    BWAPI_TYPEDEF(Order,DisableDoodad);
    BWAPI_TYPEDEF(Order,WarpIn);
    BWAPI_TYPEDEF(Order,Medic);
    BWAPI_TYPEDEF(Order,MedicHeal);
    BWAPI_TYPEDEF(Order,HealMove);
    BWAPI_TYPEDEF(Order,MedicHealToIdle);
    BWAPI_TYPEDEF(Order,CastRestoration);
    BWAPI_TYPEDEF(Order,CastDisruptionWeb);
    BWAPI_TYPEDEF(Order,CastMindControl);
    BWAPI_TYPEDEF(Order,DarkArchonMeld);
    BWAPI_TYPEDEF(Order,CastFeedback);
    BWAPI_TYPEDEF(Order,CastOpticalFlare);
    BWAPI_TYPEDEF(Order,CastMaelstrom);
    BWAPI_TYPEDEF(Order,JunkYardDog);
    BWAPI_TYPEDEF(Order,Fatal);
    BWAPI_TYPEDEF(Order,None);
    BWAPI_TYPEDEF(Order,Unknown);
  }

  Order::Order(int id) : Type( id )
  {}
  const Order::set& Orders::allOrders()
  {
    return OrderSet::orderSet;
  }
}
