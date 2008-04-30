#include "Unit.h"
#include <stdio.h>
#include "BWHandle.h"
#include "Globals.h"
#include "StringUtil.h"
#include "BWGame.h"
#include "Player.h"
#include <math.h>
//------------------------------- CONSTRUCTOR ----------------------------------
Unit::Unit(LPVOID address)
: x(0),y(0), address(address), playerID(0), workersMiningFromThis(0)
 {
  nextAddress = NULL;
  previousAddress = NULL;
  next = NULL;
  previous = NULL;
  this->update();
 }
//-------------------------------- DESTRUCTOR ----------------------------------
Unit::~Unit(void)
 {

  if (this->playerID < globalBWGame->getPlayerCount() &&
      globalBWGame->getPlayer(this->playerID)->units == this)
    globalBWGame->getPlayer(this->playerID)->units = this->next;
  if (this->next != NULL)
    this->next->previous = this->previous;
  if (this->previous != NULL)
    this->previous->next = this->next;
 }
//------------------------------------------------------------------------------
void Unit::update(void)
 {
  int tmp;
  this->hp = 0;
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x09), &this->hp, 2);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x08), &this->hpFraction, 1);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x14), &this->targetAddress, 4);

  globalBWHandle->readMemory((LPVOID)((int)(this->address) + +0x4D), &this->mainOrderID, 1);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + +0x4E), &this->mainOrderState, 1);

  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x5C), &this->orderTargetAddress, 4);

  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x60), &this->shieldFraction, 1);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x61), &this->shield, 2);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x18), &this->x, 2);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x1A), &this->y, 2);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x10), &this->moveX, 2);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x12), &this->moveY, 2);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x34), &this->acceleration, 4);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x4C), &this->playerID, 1);
  this->temp = 0;
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x51), &this->temp, 1);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x55), &this->groundWeaponCooldown, 1);

  globalBWHandle->readMemory((LPVOID)((int)(this->address)       ), &this->previousAddress, 4);
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x04), &this->nextAddress, 4);

  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0x64),&tmp, 4);
  unitID = (UnitType)tmp;
  this->resourceContained = 0;
  globalBWHandle->readMemory((LPVOID)((int)(this->address) + 0xD0),&this->resourceContained, 2);

  }
//------------------------------------------------------------------------------
std::string Unit::unitName(void)
 {
  switch (this->unitID)
   {
    case Terran_Marine: return "Terran_Marine";
    case Terran_FireBat: return "Terran_FireBat";
    case Terran_Ghost: return "Terran_Ghost";
    case Terran_Medic: return "Terran_Medic";
    case Terran_Vulture: return "Terran_Vulture";
    case Terran_Goliath: return "Terran_Goliath";
    case Terran_SeigeTankTankMode: return "Terran_SeigeTankTankMode";
    case Terran_SeigeTankSiegeMode: return "Terran_SeigeTankSiegeMode";
    case Terran_SCV: return "Terran_SCV";
    case Terran_VultureSpiderMine: return "Terran_VultureSpiderMine";
    case Terran_NuclearMissile: return "Terran_NuclearMissile";
    case Terran_Wraith: return "Terran_Wraith";
    case Terran_ScienceVessel: return "Terran_ScienceVessel";
    case Terran_DropShip: return "Terran_DropShip";
    case Terran_Battlecruiser: return "Terran_Battlecruiser";
    case Terran_Valkyrie: return "Terran_Valkyrie";
    case Terran_CommandCenter: return "Terran_CommandCenter";
    case Terran_SupplyDepot: return "Terran_SupplyDepot";
    case Terran_Refinery: return "Terran_Refinery";
    case Terran_Barracks: return "Terran_Barracks";
    case Terran_Academy: return "Terran_Academy";
    case Terran_Factory: return "Terran_Factory";
    case Terran_Starport: return "Terran_Starport";
    case Terran_ScienceFacility: return "Terran_ScienceFacility";
    case Terran_EngineeringBay: return "Terran_EngineeringBay";
    case Terran_Armory: return "Terran_Armory";
    case Terran_MissileTurret: return "Terran_MissileTurret";
    case Terran_Bunker: return "Terran_Bunker";
    case Terran_ComsatStation: return "Terran_ComsatStation";
    case Terran_NuclearSilo: return "Terran_NuclearSilo";
    case Terran_ControlTower: return "Terran_ControlTower";
    case Terran_CovertOps: return "Terran_CovertOps";
    case Terran_PhysicsLab: return "Terran_PhysicsLab";
    case Terran_MachineShop: return "Terran_MachineShop";
    case Protoss_DarkTemplar: return "Protoss_DarkTemplar";
    case Protoss_DarkArchon: return "Protoss_DarkArchon";
    case Protoss_Probe: return "Protoss_Probe";
    case Protoss_Zealot: return "Protoss_Zealot";
    case Protoss_Dragoon: return "Protoss_Dragoon";
    case Protoss_HighTemplar: return "Protoss_HighTemplar";
    case Protoss_Archon: return "Protoss_Archon";
    case Protoss_Reaver: return "Protoss_Reaver";
    case Protoss_Scarab: return "Protoss_Scarab";
    case Protoss_Corsair: return "Protoss_Corsair";
    case Protoss_Shuttle: return "Protoss_Shuttle";
    case Protoss_Scout: return "Protoss_Scout";
    case Protoss_Arbiter: return "Protoss_Arbiter";
    case Protoss_Carrier: return "Protoss_Carrier";
    case Protoss_Interceptor: return "Protoss_Interceptor";
    case Protoss_Observer: return "Protoss_Observer";
    case Protoss_Nexus: return "Protoss_Nexus";
    case Protoss_RoboticsFacility: return "Protoss_RoboticsFacility";
    case Protoss_Pylon: return "Protoss_Pylon";
    case Protoss_Assimilator: return "Protoss_Assimilator";
    case Protoss_Observatory: return "Protoss_Observatory";
    case Protoss_Gateway: return "Protoss_Gateway";
    case Protoss_PhotonCannon: return "Protoss_PhotonCannon";
    case Protoss_CitadelOfAdun: return "Protoss_CitadelOfAdun";
    case Protoss_CyberneticsCore: return "Protoss_CyberneticsCore";
    case Protoss_TemplarArchives: return "Protoss_TemplarArchives";
    case Protoss_Forge: return "Protoss_Forge";
    case Protoss_Stargate: return "Protoss_Stargate";
    case Protoss_FleetBeacon: return "Protoss_FleetBeacon";
    case Protoss_ArbiterTribunal: return "Protoss_ArbiterTribunal";
    case Protoss_RoboticsSupportBay: return "Protoss_RoboticsSupportBay";
    case Protoss_ShieldBattery: return "Protoss_ShieldBattery";
    case Zerg_Larva: return "Zerg_Larva";
    case Zerg_Egg: return "Zerg_Egg";
    case Zerg_LurkerEgg: return "Zerg_LurkerEgg";
    case Zerg_Zergling: return "Zerg_Zergling";
    case Zerg_Hydralisk: return "Zerg_Hydralisk";
    case Zerg_Ultralisk: return "Zerg_Ultralisk";
    case Zerg_Broodling: return "Zerg_Broodling";
    case Zerg_Drone: return "Zerg_Drone";
    case Zerg_Defiler: return "Zerg_Defiler";
    case Zerg_InfestedTerran: return "Zerg_InfestedTerran";
    case Zerg_Lurker: return "Zerg_Lurker";
    case Zerg_Overlord: return "Zerg_Overlord";
    case Zerg_Mutalisk: return "Zerg_Mutalisk";
    case Zerg_Guardian: return "Zerg_Guardian";
    case Zerg_Devourer: return "Zerg_Devourer";
    case Zerg_Queen: return "Zerg_Queen";
    case Zerg_Scourge: return "Zerg_Scourge";
    case Zerg_DevourerGuardianCocoon: return "Zerg_DevourerGuardianCocoon";
    case Zerg_InfestedCommandCenter: return "Zerg_InfestedCommandCenter";
    case Zerg_Hatchery: return "Zerg_Hatchery";
    case Zerg_Lair: return "Zerg_Lair";
    case Zerg_Hive: return "Zerg_Hive";
    case Zerg_NydusCanal: return "Zerg_NydusCanal";
    case Zerg_HydraliskDen: return "Zerg_HydraliskDen";
    case Zerg_DefilerMound: return "Zerg_DefilerMound";
    case Zerg_GreaterSpire: return "Zerg_GreaterSpire";
    case Zerg_QueensNest: return "Zerg_QueensNest";
    case Zerg_EvolutionChamber: return "Zerg_EvolutionChamber";
    case Zerg_UltraliskCavern: return "Zerg_UltraliskCavern";
    case Zerg_Spire: return "Zerg_Spire";
    case Zerg_SpawningPool: return "Zerg_SpawningPool";
    case Zerg_CreepColony: return "Zerg_CreepColony";
    case Zerg_SporeColony: return "Zerg_SporeColony";
    case Zerg_SunkenColony: return "Zerg_SunkenColony";
    case Zerg_Extractor: return "Zerg_Extractor";
    case Resource_MineralPatch1: return "Resource_MineralPatch1";
    case Resource_MineralPatch2: return "Resource_MineralPatch2";
    case Resource_MineralPatch3: return "Resource_MineralPatch3";
    case Resource_VespeneGeyser: return "Resource_VespeneGeyser";
    case Beacon_Zerg: return "Beacon_Zerg";
    case Beacon_Terran: return "Beacon_Terran";
    case Beacon_Protoss: return "Beacon_Protoss";
    case Beacon_ZergFlag: return "Beacon_ZergFlag";
    case Beacon_TerranFlag: return "Beacon_TerranFlag";
    case Beacon_ProtossFlag: return "Beacon_ProtossFlag";
    case Critter_Rhynadon: return "Critter_Rhynadon";
    case Critter_Bengalaas: return "Critter_Bengalaas";
    case Critter_Scantid: return "Critter_Scantid";
    case Critter_Kakaru: return "Critter_Kakaru";
    case Critter_Ragnasaur: return "Critter_Ragnasaur";
    case Critter_Ursadon: return "Critter_Ursadon";
    case Spell_ScannerSweep: return "Spell_ScannerSweep";
    case Spell_DisruptionWeb: return "Spell_DisruptionWeb";
    case Spell_DarkSwarm: return "Spell_DarkSwarm";
    default: return "Unknown (" + StringUtil::intToString(unitID) + ")";
   }
 }
//------------------------------------------------------------------------------
std::string Unit::getStateDescription(void)
 {
  switch (this->mainOrderID)
   {
    case Idle : return "Idle";
    case ApproachingMinerals : return "Approaching Minerals";
    case Moving : return "Moving";
    case Mining : return "Mining";
    case GettingMinedMinerals : return "Getting mined minerals";
    case GoingToBuild : return "Going to build";
    case ReturningMinerals : return "Returning Minerals";
    case Attacking : return "Attacking";
    case Following : return "Following";
    case AttackMoving : return "Attack Moving";
    case ApproachingRafinery : return "Approaching rafinery";
    case ReturningGas : return "ReturningGas";
    case EnteringRafinery: return "EnteringRafinery";
    case InRafinery : return "In rafinery";
    default   : return "Unknown (" + StringUtil::intToString(this->mainOrderID) + ")";
   }
 }
//------------------------------------------------------------------------------
short int Unit::getMoveX(void)
 {
  return this->moveX;
 }
//------------------------------------------------------------------------------
void Unit::setMoveX(short int moveX)
 {
  this->moveX = moveX;
  globalBWHandle->writeMemory((LPVOID)((int)(this->address) + 0x10), &this->moveX, 2);
 }
//------------------------------------------------------------------------------
short int Unit::getMoveY(void)
 {
  return this->moveY;
 }
//------------------------------------------------------------------------------
void Unit::setMoveY(short int moveY)
 {
  this->moveY = moveY;
  globalBWHandle->writeMemory((LPVOID)((int)(this->address) + 0x12), &this->moveY, 2);
 }
//------------------------------------------------------------------------------
int Unit::getHp(void)
 {
  return this->hp;
 }
//------------------------------------------------------------------------------
void Unit::setHp(int hp)
 {
  this->hp = hp;
  globalBWHandle->writeMemory((LPVOID)((int)(this->address) + 0x8), &this->hp, 4);
 }
//------------------------------------------------------------------------------
int Unit::getShield(void)
 {
  return this->shield;
 }
//------------------------------------------------------------------------------
byte Unit::getHpFraction(void)
 {
  return this->hpFraction;
 }
//------------------------------------------------------------------------------
byte Unit::getShieldFraction(void)
 {
  return this->shieldFraction;
 }
//------------------------------------------------------------------------------
Unit *Unit::getTarget(void)
 {
  return this->target;
 }
//------------------------------------------------------------------------------
void Unit::updateTarget(void)
 {
  if ((this->targetAddress - UNIT_NODE_TABLE_ADDRESS)/336 <= 1700 &&
        (this->targetAddress - UNIT_NODE_TABLE_ADDRESS)/336 > 0)
    this->target = globalBWGame->globalUnits[(this->targetAddress - UNIT_NODE_TABLE_ADDRESS)/336];
  else
    this->target = NULL;

  if ((this->orderTargetAddress - UNIT_NODE_TABLE_ADDRESS)/336 <= 1700 &&
        (this->orderTargetAddress - UNIT_NODE_TABLE_ADDRESS)/336 > 0)
    this->orderTarget = globalBWGame->globalUnits[(this->orderTargetAddress - UNIT_NODE_TABLE_ADDRESS)/336];
  else
    this->orderTarget = NULL;
 }
//------------------------------------------------------------------------------
void Unit::setGroundWeaponCooldown(byte groundWeaponCooldown)
 {
  this->groundWeaponCooldown = groundWeaponCooldown;
  globalBWHandle->writeMemory((LPVOID)((int)(this->address) + 0x55), &this->groundWeaponCooldown, 4);
 }
//------------------------------------------------------------------------------
int Unit::getAcceleration(void)
 {
  return this->acceleration;
 }
//------------------------------------------------------------------------------
void Unit::setAcceleration(int acceleration)
 {
  this->acceleration = acceleration;
  globalBWHandle->writeMemory((LPVOID)((int)(this->address) + 0x48), &this->acceleration, 4);
 }
//------------------------------------------------------------------------------
int Unit::getDistance(Unit *unit)
 {
  return sqrt((this->x - unit->x)*(this->x - unit->x) + (this->y - unit->y)*(this->y - unit->y));
 }
//------------------------------------------------------------------------------
Unit *Unit::getOrderTarget(void)
 {
  return this->orderTarget;
 }
//------------------------------------------------------------------------------
void Unit::setOrderTarget(Unit *unit)
 {
  this->orderTarget = unit;
  this->orderTargetAddress = (int) unit->address;
  globalBWHandle->writeMemory((LPVOID)((int)(this->address) + 0x5C), &unit->address, 4);
 }
//------------------------------------------------------------------------------
byte Unit::getOrderID(void)
 {
  return this->mainOrderID;
 }
//------------------------------------------------------------------------------
void Unit::setOrderID(byte orderID)
 {
  this->mainOrderID = orderID;
  globalBWHandle->writeMemory((LPVOID)((int)(this->address) + 0x48), &this->mainOrderID, 1);
 }
