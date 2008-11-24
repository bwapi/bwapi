#include "Player.h"

#include <Util/Bitmask.h>

#include <BW/Offsets.h>
#include <BW/UnitID.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Player::Player(u8 id)
    :id(id)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Player::~Player()
  {
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  char* Player::getName() const
  {
    return BW::BWXFN_Players->player[this->getID()].name;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  s32 Player::getMinerals() const
  {
    return BW::BWXFN_PlayerResources->minerals.player[this->getID()];
  }
  //------------------------------------------------ GET GAS -------------------------------------------------
  s32 Player::getGas() const
  {
    return BW::BWXFN_PlayerResources->gas.player[this->getID()];
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  u8 Player::getID() const
  {
    return this->id;
  }
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  BW::Unit** Player::selectedUnit()
  {
    return (BW::Unit**)(0x006284D0 + this->getID()*48);
  }
  #pragma warning(pop)
  //------------------------------------------- GET MINERALS LOCAL -------------------------------------------
  s32 Player::getMineralsLocal() const
  {
    return this->mineralsLocal;
  }
  //--------------------------------------------- GET GAS LOCAL ----------------------------------------------
  s32 Player::getGasLocal() const
  {
    return this->gasLocal;
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void Player::update()
  {
    this->mineralsLocal = this->getMinerals();
    this->gasLocal = this->getGas();
    for (int i = 0; i < BW::RACE_COUNT; i++)
    {
      this->suppliesAvailableLocal[i] = this->getSuppliesAvailable((BW::Race::Enum)i);
      this->suppliesUsedLocal[i] = this->getSuppliesUsed((BW::Race::Enum)i);
    }
    for (u16 j = 0; j < BW::UNIT_TYPE_COUNT; j++)
      this->toMake[j] = 0;
  }
  //---------------------------------------------- SPEND LOCAL -----------------------------------------------
  void  Player::spendLocal(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //------------------------------------------ GET SUPPLY AVAILABLE ------------------------------------------
  s32 Player::getSuppliesAvailable(BW::Race::Enum race)
  {
    s32 ret = BW::BWXFN_Supplies->race[race].available.player[this->getID()];
    return ret < getSuppliesMax(race) ? ret : getSuppliesMax(race);
  }
  //-------------------------------------------- GET SUPPLY USED ---------------------------------------------
  s32 Player::getSuppliesUsed(BW::Race::Enum race)
  {
    return BW::BWXFN_Supplies->race[race].used.player[this->getID()];
  }
  //--------------------------------------------- GET SUPPLY MAX ---------------------------------------------
  s32 Player::getSuppliesMax(BW::Race::Enum race)
  {
    return BW::BWXFN_Supplies->race[race].max.player[this->getID()];
  }
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  s32 Player::getSuppliesAvailableLocal(BW::Race::Enum race)
  {
    s32 ret = this->suppliesAvailableLocal[race];
    return ret < getSuppliesMax(race) ? ret : getSuppliesMax(race);
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  s32 Player::getSuppliesUsedLocal(BW::Race::Enum race)
  {
    return this->suppliesUsedLocal[race];
  }
  //------------------------------------------ SUPPLIES FREE LOCAL -------------------------------------------
  s32 Player::getSuppliesFreeLocal(BW::Race::Enum race)
  {
    return this->getSuppliesAvailableLocal(race) - this->getSuppliesUsedLocal(race);
  }
  //--------------------------------------- USE SUPPLIES PROTOSS LOCAL ---------------------------------------
  void Player::useSuppliesLocal(u8 supplies, BW::Race::Enum race)
  {
    this->suppliesUsedLocal[race] += supplies;
  }
  //----------------------------------------------- CAN BUILD ------------------------------------------------
  bool Player::canBuild(BW::UnitType unit)
  {
    switch (unit.getID())
    {
// Terran
     case BW::UnitID::Terran_Medic           :
     case BW::UnitID::Terran_Firebat         : return this->getCompletedUnits(BW::UnitID::Terran_Academy) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Terran_Barracks) != 0;
     case BW::UnitID::Terran_Ghost           : return this->getCompletedUnits(BW::UnitID::Terran_Academy) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Terran_CovertOps) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Terran_Barracks) != 0;
     case BW::UnitID::Terran_ComsatStation   : return this->getCompletedUnits(BW::UnitID::Terran_Academy) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Terran_CommandCenter) != 0;
     case BW::UnitID::Terran_Marine          :
     case BW::UnitID::Terran_Academy         :
     case BW::UnitID::Terran_Bunker          :
     case BW::UnitID::Terran_Factory         : return this->getCompletedUnits(BW::UnitID::Terran_Barracks) != 0;
     case BW::UnitID::Terran_Vulture         :
     case BW::UnitID::Terran_Armory          :
     case BW::UnitID::Terran_MachineShop     :
     case BW::UnitID::Terran_Starport        : return this->getCompletedUnits(BW::UnitID::Terran_Factory) != 0;
     case BW::UnitID::Terran_SiegeTankSiegeMode :
     case BW::UnitID::Terran_SiegeTankTankMode : return this->getCompletedUnits(BW::UnitID::Terran_Factory) != 0 &&
                                                        this->getCompletedUnits(BW::UnitID::Terran_MachineShop) != 0;
     case BW::UnitID::Terran_Goliath         : return this->getCompletedUnits(BW::UnitID::Terran_Factory) != 0 &&
                                                        this->getCompletedUnits(BW::UnitID::Terran_Armory) != 0;
     case BW::UnitID::Terran_ControlTower    :
     case BW::UnitID::Terran_Wraith          :
     case BW::UnitID::Terran_ScienceFacility : return this->getCompletedUnits(BW::UnitID::Terran_Starport) != 0;
     case BW::UnitID::Terran_Dropship        : return this->getCompletedUnits(BW::UnitID::Terran_ControlTower) != 0 && 
                                                      this->getCompletedUnits(BW::UnitID::Terran_Starport) != 0;
     case BW::UnitID::Terran_Battlecruiser   : return this->getCompletedUnits(BW::UnitID::Terran_ControlTower) != 0 && 
                                                      this->getCompletedUnits(BW::UnitID::Terran_Starport) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Terran_PhysicsLab) != 0;
     case BW::UnitID::Terran_ScienceVessel   : return this->getCompletedUnits(BW::UnitID::Terran_ControlTower) != 0 && 
                                                      this->getCompletedUnits(BW::UnitID::Terran_Starport) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Terran_ScienceFacility) != 0;
     case BW::UnitID::Terran_Valkyrie        : return this->getCompletedUnits(BW::UnitID::Terran_ControlTower) != 0 && 
                                                      this->getCompletedUnits(BW::UnitID::Terran_Starport) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Terran_Armory) != 0;
     case BW::UnitID::Terran_PhysicsLab      :
     case BW::UnitID::Terran_CovertOps       : return this->getCompletedUnits(BW::UnitID::Terran_ScienceFacility) != 0;
     case BW::UnitID::Terran_SCV             :
	 case BW::UnitID::Terran_Barracks        :
	 case BW::UnitID::Terran_EngineeringBay  : return this->getCompletedUnits(BW::UnitID::Terran_CommandCenter) != 0;
     case BW::UnitID::Terran_MissileTurret   : return this->getCompletedUnits(BW::UnitID::Terran_EngineeringBay) != 0;
// Protoss
     case BW::UnitID::Protoss_Probe          :
     case BW::UnitID::Protoss_Forge          : 
     case BW::UnitID::Protoss_Gateway        : return this->getCompletedUnits(BW::UnitID::Protoss_Nexus) != 0;
     case BW::UnitID::Protoss_PhotonCannon   : return this->getCompletedUnits(BW::UnitID::Protoss_Forge) != 0;
     case BW::UnitID::Protoss_Zealot         :
     case BW::UnitID::Protoss_CyberneticsCore:
     case BW::UnitID::Protoss_ShieldBattery  : return this->getCompletedUnits(BW::UnitID::Protoss_Gateway) != 0;
     case BW::UnitID::Protoss_HighTemplar    : 
     case BW::UnitID::Protoss_DarkTemplar    : return this->getCompletedUnits(BW::UnitID::Protoss_Gateway) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Protoss_TemplarArchives) != 0;
     case BW::UnitID::Protoss_CitadelOfAdun  :
     case BW::UnitID::Protoss_Stargate       :
     case BW::UnitID::Protoss_RoboticsFacility : return this->getCompletedUnits(BW::UnitID::Protoss_CyberneticsCore) != 0;
     case BW::UnitID::Protoss_Dragoon        : return this->getCompletedUnits(BW::UnitID::Protoss_CyberneticsCore) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Protoss_Gateway) != 0;
     case BW::UnitID::Protoss_Shuttle        :
     case BW::UnitID::Protoss_Observatory    :
     case BW::UnitID::Protoss_RoboticsSupportBay : return this->getCompletedUnits(BW::UnitID::Protoss_RoboticsFacility) != 0;
     case BW::UnitID::Protoss_Reaver         : return this->getCompletedUnits(BW::UnitID::Protoss_RoboticsFacility) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Protoss_RoboticsSupportBay) != 0;
     case BW::UnitID::Protoss_Observer       : return this->getCompletedUnits(BW::UnitID::Protoss_RoboticsFacility) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Protoss_Observatory) != 0;
     case BW::UnitID::Protoss_Scout          :
     case BW::UnitID::Protoss_Corsair        :
     case BW::UnitID::Protoss_FleetBeacon    : return this->getCompletedUnits(BW::UnitID::Protoss_Stargate) != 0;
     case BW::UnitID::Protoss_Carrier        : return this->getCompletedUnits(BW::UnitID::Protoss_Stargate) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Protoss_FleetBeacon) !=0;
     case BW::UnitID::Protoss_Arbiter        : return this->getCompletedUnits(BW::UnitID::Protoss_Stargate) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Protoss_ArbiterTribunal) !=0;
     case BW::UnitID::Protoss_TemplarArchives: return this->getCompletedUnits(BW::UnitID::Protoss_CitadelOfAdun) != 0;
     case BW::UnitID::Protoss_ArbiterTribunal: return this->getCompletedUnits(BW::UnitID::Protoss_FleetBeacon) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Protoss_TemplarArchives) != 0;
//Zerg
     case BW::UnitID::Zerg_Overlord          : 
     case BW::UnitID::Zerg_Drone             : return this->getCompletedUnits(BW::UnitID::Zerg_Larva) != 0;
     case BW::UnitID::Zerg_SpawningPool      :
     case BW::UnitID::Zerg_CreepColony       :
     case BW::UnitID::Zerg_EvolutionChamber  : return this->getCompletedUnits(BW::UnitID::Zerg_Main) != 0;
     case BW::UnitID::Zerg_Spire             :
     case BW::UnitID::Zerg_QueensNest        : return this->getCompletedUnits(BW::UnitID::Zerg_MainLair) != 0;
     case BW::UnitID::Zerg_DefilerMound      :
     case BW::UnitID::Zerg_NydusCanal        :
     case BW::UnitID::Zerg_UltraliskCavern   : return this->getCompletedUnits(BW::UnitID::Zerg_Hive) != 0;
     case BW::UnitID::Zerg_GreaterSpire      : return this->getCompletedUnits(BW::UnitID::Zerg_Hive) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_Spire) != 0;
     case BW::UnitID::Zerg_SunkenColony      : return this->getCompletedUnits(BW::UnitID::Zerg_CreepColony) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_SpawningPool) != 0;
     case BW::UnitID::Zerg_SporeColony       : return this->getCompletedUnits(BW::UnitID::Zerg_CreepColony) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_EvolutionChamber) != 0;
     case BW::UnitID::Zerg_Lair              :return this->getCompletedUnits(BW::UnitID::Zerg_SpawningPool) != 0 &&
                                                     this->getCompletedUnits(BW::UnitID::Zerg_Hatchery) != 0;
     case BW::UnitID::Zerg_Hive              :return this->getCompletedUnits(BW::UnitID::Zerg_QueensNest) != 0 &&
                                                     this->getCompletedUnits(BW::UnitID::Zerg_Lair) != 0;
     case BW::UnitID::Zerg_HydraliskDen      : return this->getCompletedUnits(BW::UnitID::Zerg_SpawningPool) != 0;
     case BW::UnitID::Zerg_Zergling          : return this->getCompletedUnits(BW::UnitID::Zerg_Larva) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_SpawningPool) != 0;
     case BW::UnitID::Zerg_Hydralisk         : return this->getCompletedUnits(BW::UnitID::Zerg_Larva) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_HydraliskDen) != 0;
     case BW::UnitID::Zerg_Scourge           :
     case BW::UnitID::Zerg_Mutalisk          : return this->getCompletedUnits(BW::UnitID::Zerg_Larva) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_Spire) != 0;
     case BW::UnitID::Zerg_Devourer          :
     case BW::UnitID::Zerg_Guardian          : return this->getCompletedUnits(BW::UnitID::Zerg_Mutalisk) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_GreaterSpire) != 0;
     case BW::UnitID::Zerg_Queen             : return this->getCompletedUnits(BW::UnitID::Zerg_Larva) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_QueensNest) != 0;
     case BW::UnitID::Zerg_Defiler           : return this->getCompletedUnits(BW::UnitID::Zerg_Larva) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_DefilerMound) != 0;
     case BW::UnitID::Zerg_Ultralisk         : return this->getCompletedUnits(BW::UnitID::Zerg_Larva) != 0 &&
                                                      this->getCompletedUnits(BW::UnitID::Zerg_UltraliskCavern) != 0;
     case BW::UnitID::Zerg_InfestedTerran    : return this->getCompletedUnits(BW::UnitID::Zerg_InfestedCommandCenter) != 0;
     case BW::UnitID::Zerg_Lurker            : return this->getCompletedUnits(BW::UnitID::Zerg_Hydralisk) != 0 &&
                                                      this->techResearched(BW::TechID::LurkerAspect);
     default                                 : return true;
    }
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::UnitType unit, const BWAPI::ReservedResources& reserved)
  {
    return ((int)this->getSuppliesFreeLocal(unit.getRace())) - reserved.supply   >= unit.getSupplies() &&
           ((int)this->getMineralsLocal())                   - reserved.minerals >= unit.getMineralPrice() &&
           ((int)this->getGasLocal())                        - reserved.gas      >= unit.getGasPrice();
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::TechType tech, const BWAPI::ReservedResources& reserved)
  {
    return ((int)this->getMineralsLocal()) - reserved.minerals >= tech.getMineralPrice() &&
           ((int)this->getGasLocal())      - reserved.gas      >= tech.getGasPrice();
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::UpgradeType upgrade, u8 level, const BWAPI::ReservedResources& reserved)
  {
    return ((int)this->getMineralsLocal()) - reserved.minerals >= upgrade.mineralCostBase() + upgrade.mineralCostFactor()*(level-1) &&
           ((int)this->getGasLocal())      - reserved.gas      >= upgrade.gasCostBase()     + upgrade.gasCostFactor()    *(level-1);
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BW::Race::Enum Player::getRace()
  {
    return BW::BWXFN_Players->player[this->getID()].race;
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  u8 Player::getForce()
  {
    return BW::BWXFN_Players->player[this->getID()].force;
  }
  //--------------------------------------------- GET ALL UNITS ----------------------------------------------
  s32 Player::getAllUnits(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWXFN_Counts->all, unit);
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 Player::getCompletedUnits(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWXFN_Counts->completed, unit);
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 Player::getCompletedUnits(BW::UnitType unit, BW::Race::Enum race)
  {
    return this->evaluateCounts(BW::BWXFN_Counts->completed, unit, race);
  }  
  //------------------------------------------ GET INCOMPLETE UNITS ------------------------------------------
  s32 Player::getIncompleteUnits(BW::UnitType unit)
  {
    return this->getAllUnits(unit) - this->getCompletedUnits(unit);
  }
  //--------------------------------------------- GET ALL UNITS ----------------------------------------------
  s32 Player::getAllUnitsLocal(BW::UnitType unit)
  {
    return this->getAllUnits(unit) + this->toMake[unit.getID()];
  }
  //------------------------------------------ GET INCOMPLETE UNITS ------------------------------------------
  s32 Player::getIncompleteUnitsLocal(BW::UnitType unit)
  {
    return this->getIncompleteUnits(unit) + toMake[unit.getID()];
  }
  //----------------------------------------------- GET DEATHS -----------------------------------------------
  s32 Player::getDeaths(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWXFN_Counts->dead, unit);
  }
  //----------------------------------------------- GET KILLS ------------------------------------------------
  s32 Player::getKills(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWXFN_Counts->killed, unit);
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  s32 Player::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit)
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Infantry)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canAttack() &&
			BW::UnitType((BW::UnitID::Enum)i).isOrganic() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Mech)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isMechanical() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_Main)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Hatchery ||
            unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_MainLair)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += counts.unit[i].player[this->getID()];
    return temp;
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  s32 Player::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit, BW::Race::Enum race)
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

	    if (unit == BW::UnitID::Infantry)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canAttack() &&
			BW::UnitType((BW::UnitID::Enum)i).isOrganic() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Mech)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
		  if (BW::UnitType((BW::UnitID::Enum)i).isMechanical() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_Main)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Hatchery ||
            unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_MainLair)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];
    return temp;
  }  
  //------------------------------------------- PLAN TO MAKE -------------------------------------------------
  void Player::planToMake(BW::UnitType unit)
  {
    this->toMake[unit.getID()]++;
  }
  //------------------------------------------- GET FORCE NAME -----------------------------------------------
  char* Player::getForceName() const
  {
    return BW::ForceNames[BW::BWXFN_Players->player[this->getID()].force].name;
  }
  //------------------------------------------ RESEARCH IN PROGRESS ------------------------------------------
  bool Player::researchInProgress(BW::TechType tech) const
  {
    Util::BitMask<u64>* techs = (Util::BitMask<u64>*) (0x0058F218 + this->getID()*6);
    return techs->getBit(1 << tech.getID());
  }
  //-------------------------------------------- TECH RESEARCHED ---------------------------------------------
  bool Player::techResearched(BW::TechType tech) const
  {
   if (tech.getID() < 0x18)
     return *((u8*)(0x0058CF2C + this->getID()*0x18 + tech.getID())) == 1;
   else
     return *((u8*)(0x0058F128 + this->getID()*0x14 + tech.getID() - 0x18)) == 1;
  }
  //--------------------------------------------- UPGRADE LEVEL ----------------------------------------------
  u8 Player::upgradeLevel(BW::UpgradeType upgrade) const
  {
    if (upgrade.getID() < 46)
     return *((u8*)(0x0058D298 + this->getID()*46 + upgrade.getID()));
   else
     return *((u8*)(0x0058F314 + this->getID()*15 + upgrade.getID() - 46));
  }
  //------------------------------------------ UPGRADE IN PROGRESS -------------------------------------------
  bool Player::upgradeInProgress(BW::UpgradeType upgrade) const
  {
    return BW::BWXFN_UpgradeProgress->player[this->getID()].getBit(1 << upgrade.getID());
  }  
  //----------------------------------------------------------------------------------------------------------
};
