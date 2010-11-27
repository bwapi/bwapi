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
    extern const Order Nothing3;
    extern const Order CastInfestation;
    extern const Order InfestingCommandCenter;
    extern const Order PlaceBuilding;
    extern const Order BuildProtoss2;
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
    extern const Order CompletingArchonsummon;
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
    extern const Order NukeUnit;
    extern const Order CastNuclearStrike;
    extern const Order NukeTrack;
    extern const Order CloakNearbyUnits;
    extern const Order PlaceMine;
    extern const Order RightClickAction;
    extern const Order CastRecall;
    extern const Order TeleporttoLocation;
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
    extern const Order MedicHeal2;
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
}
