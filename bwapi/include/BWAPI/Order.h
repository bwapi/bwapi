#pragma once
#include <string>
#include <set>
namespace BWAPI
{
  /** To get detailed information about what a unit is doing, you can use the Unit::getOrder method, which
   * will return an Order object. Note that a single command, like gather minerals, can consist of several
   * orders ( MoveToMinerals, HarvestMinerals2, MiningMinerals, ReturnMinerals, etc) which will indicate what
   * state the unit is in while executing the command. For information about how to issue commands to units,
   * go to Unit. */
  class Order
  {
    public:
      Order();
      Order(int id);
      Order(const Order& other);
      Order& operator=(const Order& other);
      bool operator==(const Order& other) const;
      bool operator!=(const Order& other) const;
      bool operator<(const Order& other) const;

      /** Returns the unique ID for this order. */
      int getID() const;

      /** Returns the name of this order. */
      std::string getName() const;
    private:
      int id;
  };
  namespace Orders
  {
    /** Given the name of an order, getOrder() will return the corresponding order object. */
    Order getOrder(std::string name);

    /** Returns the set of all the Orders. */
    std::set<Order>& allOrders();

    void init();
    extern const Order Die;
    extern const Order Stop;
    extern const Order Guard;
    extern const Order PlayerGuard;
    extern const Order TurretGuard;
    extern const Order BunkerGuard;
    extern const Order Move;
    extern const Order ReaverStop;
    extern const Order Attack1;
    extern const Order Attack2;
    extern const Order AttackUnit;
    extern const Order AttackFixedRange;
    extern const Order AttackTile;
    extern const Order Hover;
    extern const Order AttackMove;
    extern const Order InfestMine1;
    extern const Order UnusedNothing;
    extern const Order UnusedPowerup;
    extern const Order TowerGuard;
    extern const Order TowerAttack;
    extern const Order VultureMine;
    extern const Order StayinRange;
    extern const Order TurretAttack;
    extern const Order Nothing;
    extern const Order Nothing3;
    extern const Order DroneStartBuild;
    extern const Order DroneBuild;
    extern const Order InfestMine2;
    extern const Order InfestMine3;
    extern const Order InfestMine4;
    extern const Order BuildTerran;
    extern const Order BuildProtoss1;
    extern const Order BuildProtoss2;
    extern const Order ConstructingBuilding;
    extern const Order Repair1;
    extern const Order Repair2;
    extern const Order PlaceAddon;
    extern const Order BuildAddon;
    extern const Order Train;
    extern const Order RallyPointUnit;
    extern const Order RallyPointTile;
    extern const Order ZergBirth;
    extern const Order ZergUnitMorph;
    extern const Order ZergBuildingMorph;
    extern const Order TerranBuildSelf;
    extern const Order ZergBuildSelf;
    extern const Order BuildNydusExit;
    extern const Order EnterNydusCanal;
    extern const Order ProtossBuildSelf;
    extern const Order Follow;
    extern const Order Carrier;
    extern const Order ReaverCarrierMove;
    extern const Order CarrierStop;
    extern const Order CarrierAttack1;
    extern const Order CarrierAttack2;
    extern const Order CarrierIgnore2;
    extern const Order CarrierFight;
    extern const Order CarrierHoldPosition;
    extern const Order Reaver;
    extern const Order ReaverAttack1;
    extern const Order ReaverAttack2;
    extern const Order ReaverFight;
    extern const Order ReaverHoldPosition;
    extern const Order TrainFighter;
    extern const Order StrafeUnit1;
    extern const Order StrafeUnit2;
    extern const Order RechargeShieldsUnit;
    extern const Order RechargeShieldsBattery;
    extern const Order ShieldBattery;
    extern const Order Return;
    extern const Order DroneLand;
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
    extern const Order SiegeMode;
    extern const Order TankMode;
    extern const Order WatchTarget;
    extern const Order InitCreepGrowth;
    extern const Order SpreadCreep;
    extern const Order StoppingCreepGrowth;
    extern const Order GuardianAspect;
    extern const Order WarpingArchon;
    extern const Order CompletingArchonsummon;
    extern const Order HoldPosition;
    extern const Order QueenHoldPosition;
    extern const Order Cloak;
    extern const Order Decloak;
    extern const Order Unload;
    extern const Order MoveUnload;
    extern const Order FireYamatoGun1;
    extern const Order FireYamatoGun2;
    extern const Order MagnaPulse;
    extern const Order Burrow;
    extern const Order Burrowed;
    extern const Order Unburrow;
    extern const Order DarkSwarm;
    extern const Order CastParasite;
    extern const Order SummonBroodlings;
    extern const Order EmpShockwave;
    extern const Order NukeWait;
    extern const Order NukeTrain;
    extern const Order NukeLaunch;
    extern const Order NukePaint;
    extern const Order NukeUnit;
    extern const Order NukeGround;
    extern const Order NukeTrack;
    extern const Order InitArbiter;
    extern const Order CloakNearbyUnits;
    extern const Order PlaceMine;
    extern const Order Rightclickaction;
    extern const Order SapUnit;
    extern const Order SapLocation;
    extern const Order SuicideHoldPosition;
    extern const Order Teleport;
    extern const Order TeleporttoLocation;
    extern const Order PlaceScanner;
    extern const Order Scanner;
    extern const Order DefensiveMatrix;
    extern const Order PsiStorm;
    extern const Order Irradiate;
    extern const Order Plague;
    extern const Order Consume;
    extern const Order Ensnare;
    extern const Order StasisField;
    extern const Order Hallucination1;
    extern const Order Hallucination2;
    extern const Order ResetCollision1;
    extern const Order ResetCollision2;
    extern const Order Patrol;
    extern const Order CTFCOPInit;
    extern const Order CTFCOP1;
    extern const Order CTFCOP2;
    extern const Order ComputerAI;
    extern const Order AtkMoveEP;
    extern const Order HarassMove;
    extern const Order AIPatrol;
    extern const Order GuardPost;
    extern const Order RescuePassive;
    extern const Order Neutral;
    extern const Order ComputerReturn;
    extern const Order InitPsiProvider;
    extern const Order SelfDestrucing;
    extern const Order Critter;
    extern const Order HiddenGun;
    extern const Order OpenDoor;
    extern const Order CloseDoor;
    extern const Order HideTrap;
    extern const Order RevealTrap;
    extern const Order Enabledoodad;
    extern const Order Disabledoodad;
    extern const Order Warpin;
    extern const Order Medic;
    extern const Order MedicHeal1;
    extern const Order HealMove;
    extern const Order MedicHoldPosition;
    extern const Order MedicHeal2;
    extern const Order Restoration;
    extern const Order CastDisruptionWeb;
    extern const Order CastMindControl;
    extern const Order WarpingDarkArchon;
    extern const Order CastFeedback;
    extern const Order CastOpticalFlare;
    extern const Order CastMaelstrom;
    extern const Order JunkYardDog;
    extern const Order Fatal;
    extern const Order None;
    extern const Order Unknown;
  };
}
