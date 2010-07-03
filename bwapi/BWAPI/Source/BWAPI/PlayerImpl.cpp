#include "PlayerImpl.h"
#include "GameImpl.h"
#include "UnitImpl.h"

#include <string>
#include <Util/Bitmask.h>

#include <BW/Offsets.h>
#include <BW/UnitID.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PlayerImpl::PlayerImpl(u8 id)
      : id(id), leftTheGame(false)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  PlayerImpl::~PlayerImpl()
  {
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  int PlayerImpl::getID() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return this->id;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string PlayerImpl::getName() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getID() == 11)
    {
      return std::string("Neutral");
    }
    return std::string(BW::BWDATA_Players->player[this->getID()].szName);
  }
  //----------------------------------------------- GET UNITS ------------------------------------------------
  const std::set<Unit*>& PlayerImpl::getUnits() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return this->units;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BWAPI::Race PlayerImpl::getRace() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BWAPI::Race((int)(BW::BWDATA_Players->player[this->getID()].nRace));
  }
  //----------------------------------------------- PLAYER TYPE ----------------------------------------------
  BWAPI::PlayerType PlayerImpl::getType() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BWAPI::PlayerType((int)(BW::BWDATA_Players->player[this->getID()].nType));
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  Force* PlayerImpl::getForce() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return (Force*)this->force;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isAlly(Player* player) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (player==NULL || this->isNeutral() || player->isNeutral()) return false;
    return BW::BWDATA_Alliance->alliance[this->getID()].player[((PlayerImpl*)player)->getID()] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isEnemy(Player* player) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (player==NULL || this->isNeutral() || player->isNeutral()) return false;
    return BW::BWDATA_Alliance->alliance[this->getID()].player[((PlayerImpl*)player)->getID()] == 0;
  }
  //----------------------------------------------- IS NEUTRAL -----------------------------------------------
  bool PlayerImpl::isNeutral() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return this->getID() == 11;
  }
  //------------------------------------------- GET START POSITION -------------------------------------------
  TilePosition PlayerImpl::getStartLocation() const
  {
    /* error checking */
    BroodwarImpl.setLastError(Errors::None);
    if (this->isNeutral())
      return TilePositions::None;
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return TilePositions::Unknown;
    }
    /* return the start location as a tile position */
    return BWAPI::TilePosition((int)((BW::BWDATA_startPositions[this->getID()].x - BW::TILE_SIZE * 2) / BW::TILE_SIZE),
                               (int)((BW::BWDATA_startPositions[this->getID()].y - (int)(BW::TILE_SIZE * 1.5)) / BW::TILE_SIZE));
  }
  //------------------------------------------------ MINERALS ------------------------------------------------
  int PlayerImpl::minerals() const
  {
    /* error handling */
    BroodwarImpl.setLastError(Errors::None);
    if (this->isNeutral())
      return 0;
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    /* return the local mineral count */
    return this->mineralsLocal;
  }
  //-------------------------------------------------- GAS ---------------------------------------------------
  int PlayerImpl::gas() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->isNeutral())
      return 0;
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->gasLocal;
  }
  //------------------------------------------ CUMULATIVE MINERALS -------------------------------------------
  int PlayerImpl::cumulativeMinerals() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->isNeutral())
      return 0;
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return BW::BWDATA_PlayerResources->cumulativeMinerals.player[this->getID()];
  }
  //--------------------------------------------- CUMULATIVE GAS ---------------------------------------------
  int PlayerImpl::cumulativeGas() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->isNeutral())
      return 0;
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return BW::BWDATA_PlayerResources->cumulativeGas.player[this->getID()];
  }
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  int PlayerImpl::supplyTotal() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    int ret = this->suppliesAvailableLocal[getRace().getID()];
    return ret < getSuppliesMaxSync((u8)getRace().getID()) ? ret : getSuppliesMaxSync((u8)getRace().getID());
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int PlayerImpl::supplyUsed() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->suppliesUsedLocal[getRace().getID()];
  }
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  int PlayerImpl::supplyTotal(Race race) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    int ret = this->suppliesAvailableLocal[race.getID()];
    return ret < getSuppliesMaxSync((u8)race.getID()) ? ret : getSuppliesMaxSync((u8)race.getID());
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->suppliesUsedLocal[race.getID()];
  }
  //--------------------------------------------- GET ALL UNITS ----------------------------------------------
  int PlayerImpl::allUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (unit==UnitTypes::Unknown || unit==UnitTypes::None) return 0;
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->all, BW::UnitType((u16)unit.getID())) + this->toMake[unit.getID()];
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  int PlayerImpl::completedUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (unit==UnitTypes::Unknown || unit==UnitTypes::None) return 0;
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->completed, BW::UnitType((u16)unit.getID()));
  }
  //------------------------------------------ GET INCOMPLETE UNITS ------------------------------------------
  int PlayerImpl::incompleteUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (unit==UnitTypes::Unknown || unit==UnitTypes::None) return 0;
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->allUnitCount(unit) - this->completedUnitCount(unit) + toMake[unit.getID()];
  }
  //----------------------------------------------- GET DEATHS -----------------------------------------------
  int PlayerImpl::deadUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (unit==UnitTypes::Unknown || unit==UnitTypes::None) return 0;
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->dead, BW::UnitType((u16)unit.getID()));
  }
  //----------------------------------------------- GET KILLS ------------------------------------------------
  int PlayerImpl::killedUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (unit==UnitTypes::Unknown || unit==UnitTypes::None) return 0;
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->killed, BW::UnitType((u16)unit.getID()));
  }
  //------------------------------------------ RESEARCH IN PROGRESS ------------------------------------------
  bool PlayerImpl::isResearching(BWAPI::TechType tech) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return false;
    }
    Util::BitMask<u64>* techs = (Util::BitMask<u64>*) (BW::BWDATA_ResearchProgress + this->getID() * 6);
    return techs->getBit(1 << tech.getID());
  }
  //-------------------------------------------- TECH RESEARCHED ---------------------------------------------
  bool PlayerImpl::hasResearched(BWAPI::TechType tech) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return false;
    }
    if (tech.whatResearches()==UnitTypes::None)
    {
      return true;
    }
    if (tech.getID() < 0x18)
      return *((u8*)(BW::BWDATA_TechResearchSC + this->getID() * 0x18 + tech.getID())) == 1;
    else
      return *((u8*)(BW::BWDATA_TechResearchBW + this->getID() * 0x14 + tech.getID() - 0x18)) == 1;
  }
  //------------------------------------------ UPGRADE IN PROGRESS -------------------------------------------
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return false;
    }
    return BW::BWDATA_UpgradeProgress->player[this->getID()].getBit(1 << upgrade.getID());
  }
  //--------------------------------------------- UPGRADE LEVEL ----------------------------------------------
  int PlayerImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    if (upgrade.getID() < 46)
      return (int)(*((u8*)(BW::BWDATA_UpgradeLevelSC + this->getID() * 46 + upgrade.getID())));
    else
      return (int)(*((u8*)(BW::BWDATA_UpgradeLevelBW + this->getID() * 15 + upgrade.getID() - 46)));
  }
  int PlayerImpl::_getUpgradeLevel(u8 id)
  {
    if (id >= BW::UPGRADE_TYPE_COUNT)
      return 0;

    if (id < 46)
      return (int)(*((u8*)(BW::BWDATA_UpgradeLevelSC + this->getID() * 46 + id)));
    else
      return (int)(*((u8*)(BW::BWDATA_UpgradeLevelBW + this->getID() * 15 + id - 46)));
  }
  //---------------------------------------------- MAX ENERGY ------------------------------------------------
  int PlayerImpl::maxEnergy(UnitType unit) const
  {
    int energy = unit.maxEnergy();
    if ((unit == UnitTypes::Protoss_Arbiter       && getUpgradeLevel(UpgradeTypes::Khaydarin_Core)>0) ||
        (unit == UnitTypes::Protoss_Corsair       && getUpgradeLevel(UpgradeTypes::Argus_Jewel)>0) ||
        (unit == UnitTypes::Protoss_Dark_Archon   && getUpgradeLevel(UpgradeTypes::Argus_Talisman)>0) ||
        (unit == UnitTypes::Protoss_High_Templar  && getUpgradeLevel(UpgradeTypes::Khaydarin_Amulet)>0) ||
        (unit == UnitTypes::Terran_Ghost          && getUpgradeLevel(UpgradeTypes::Moebius_Reactor)>0) ||
        (unit == UnitTypes::Terran_Battlecruiser  && getUpgradeLevel(UpgradeTypes::Colossus_Reactor)>0) ||
        (unit == UnitTypes::Terran_Science_Vessel && getUpgradeLevel(UpgradeTypes::Titan_Reactor)>0) ||
        (unit == UnitTypes::Terran_Wraith         && getUpgradeLevel(UpgradeTypes::Apollo_Reactor)>0) ||
        (unit == UnitTypes::Terran_Medic          && getUpgradeLevel(UpgradeTypes::Caduceus_Reactor)>0) ||
        (unit == UnitTypes::Zerg_Defiler          && getUpgradeLevel(UpgradeTypes::Metasynaptic_Node)>0) ||
        (unit == UnitTypes::Zerg_Queen            && getUpgradeLevel(UpgradeTypes::Gamete_Meiosis)>0))
      energy += 50;
    return energy;
  }
  //---------------------------------------------- TOP SPEED -------------------------------------------------
  double PlayerImpl::topSpeed(UnitType unit) const
  {
    double speed = unit.topSpeed();
    if ((unit == UnitTypes::Terran_Vulture   && getUpgradeLevel(UpgradeTypes::Ion_Thrusters)>0) ||
        (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Pneumatized_Carapace)>0) ||
        (unit == UnitTypes::Zerg_Zergling    && getUpgradeLevel(UpgradeTypes::Metabolic_Boost)>0) ||
        (unit == UnitTypes::Zerg_Hydralisk   && getUpgradeLevel(UpgradeTypes::Muscular_Augments)>0) ||
        (unit == UnitTypes::Protoss_Zealot   && getUpgradeLevel(UpgradeTypes::Leg_Enhancements)>0) ||
        (unit == UnitTypes::Protoss_Shuttle  && getUpgradeLevel(UpgradeTypes::Gravitic_Drive)>0) ||
        (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Gravitic_Boosters)>0) ||
        (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Gravitic_Thrusters)>0) ||
        (unit == UnitTypes::Zerg_Ultralisk   && getUpgradeLevel(UpgradeTypes::Anabolic_Synthesis)>0))
      speed += 0;//?

    return speed;
  }
  //----------------------------------------- GROUND WEAPON MAX RANGE ----------------------------------------
  int PlayerImpl::groundWeaponMaxRange(UnitType unit) const
  {
    int range = unit.groundWeapon().maxRange();
    if ((unit == UnitTypes::Terran_Marine  && getUpgradeLevel(UpgradeTypes::U_238_Shells)>0) ||
        (unit == UnitTypes::Zerg_Hydralisk && getUpgradeLevel(UpgradeTypes::Grooved_Spines)>0))
      range += 1*32;
    if (unit == UnitTypes::Protoss_Dragoon && getUpgradeLevel(UpgradeTypes::Singularity_Charge)>0)
      range += 2*32;
    return range;
  }
  //------------------------------------------ AIR WEAPON MAX RANGE ------------------------------------------
  int PlayerImpl::airWeaponMaxRange(UnitType unit) const
  {
    int range = unit.airWeapon().maxRange();
    if ((unit == UnitTypes::Terran_Marine  && getUpgradeLevel(UpgradeTypes::U_238_Shells)>0) ||
        (unit == UnitTypes::Zerg_Hydralisk && getUpgradeLevel(UpgradeTypes::Grooved_Spines)>0))
      range += 1*32;
    if (unit == UnitTypes::Protoss_Dragoon && getUpgradeLevel(UpgradeTypes::Singularity_Charge)>0)
      range += 2*32;
    if (unit == UnitTypes::Terran_Goliath  && getUpgradeLevel(UpgradeTypes::Charon_Boosters)>0)
      range += 2*32;
    return range;
  }
  //--------------------------------------------- SIGHT RANGE ------------------------------------------------
  int PlayerImpl::sightRange(UnitType unit) const
  {
    int range = unit.sightRange();
    if ((unit == UnitTypes::Terran_Ghost     && getUpgradeLevel(UpgradeTypes::Ocular_Implants)>0) ||
        (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Antennae)>0) ||
        (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Sensor_Array)>0) ||
        (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Apial_Sensors)>0))
      range+=2*32;
    return range;
  }
  //-------------------------------------- GROUND WEAPON DAMAGE COOLDOWN -------------------------------------
  int PlayerImpl::groundWeaponDamageCooldown(UnitType unit) const
  {
    int cooldown = unit.groundWeapon().damageCooldown();
    if (unit == UnitTypes::Zerg_Zergling && getUpgradeLevel(UpgradeTypes::Adrenal_Glands)>0)
      cooldown -= 0;//?
    return cooldown;
  }
  //------------------------------------------------ ARMOR ---------------------------------------------------
  int PlayerImpl::armor(UnitType unit) const
  {
    int armor = unit.armor();
    armor += getUpgradeLevel(unit.armorUpgrade());
    if (unit == UnitTypes::Zerg_Ultralisk && getUpgradeLevel(UpgradeTypes::Chitinous_Plating)>0)
      armor += 2;
    return armor;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  int PlayerImpl::getMineralsSync() const
  {
    return BW::BWDATA_PlayerResources->minerals.player[this->getID()];
  }
  //------------------------------------------------ GET GAS -------------------------------------------------
  int PlayerImpl::getGasSync() const
  {
    return BW::BWDATA_PlayerResources->gas.player[this->getID()];
  }
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
  BW::Unit** PlayerImpl::selectedUnit()
  {
    return (BW::Unit**)(BW::BWDATA_PlayerSelection + this->getID() * 48);
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void PlayerImpl::update()
  {
    this->mineralsLocal = this->getMineralsSync();
    this->gasLocal = this->getGasSync();
    for (u8 i = 0; i < BW::RACE_COUNT; i++)
    {
      this->suppliesAvailableLocal[i] = this->getSuppliesAvailableSync(i);
      this->suppliesUsedLocal[i] = this->getSuppliesUsedSync(i);
    }
    if (BW::BWDATA_Players->player[this->getID()].nType  == BW::PlayerType::PlayerLeft ||
        BW::BWDATA_Players->player[this->getID()].nType  == BW::PlayerType::ComputerLeft ||
       (BW::BWDATA_Players->player[this->getID()].nType  == BW::PlayerType::Neutral && !this->isNeutral()))
    {
      this->leftTheGame = true;
    }
    for (u16 j = 0; j < BW::UNIT_TYPE_COUNT; j++)
      this->toMake[j] = 0;
  }
  //---------------------------------------------- SPEND LOCAL -----------------------------------------------
  void  PlayerImpl::spend(int minerals, int gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //------------------------------------------ GET SUPPLY AVAILABLE ------------------------------------------
  int PlayerImpl::getSuppliesAvailableSync(u8 race) const
  {
    int ret = BW::BWDATA_Supplies->race[race].available.player[this->getID()];
    return ret < getSuppliesMaxSync(race) ? ret : getSuppliesMaxSync(race);
  }
  //-------------------------------------------- GET SUPPLY USED ---------------------------------------------
  int PlayerImpl::getSuppliesUsedSync(u8 race) const
  {
    return BW::BWDATA_Supplies->race[race].used.player[this->getID()];
  }
  //--------------------------------------------- GET SUPPLY MAX ---------------------------------------------
  int PlayerImpl::getSuppliesMaxSync(u8 race) const
  {
    return BW::BWDATA_Supplies->race[race].max.player[this->getID()];
  }
  //--------------------------------------- USE SUPPLIES PROTOSS LOCAL ---------------------------------------
  void PlayerImpl::useSupplies(int supplies, u8 race)
  {
    this->suppliesUsedLocal[race] += supplies;
  }
  //------------------------------------------------ GET ALLIANCE --------------------------------------------
  u8 PlayerImpl::getAlliance(u8 opposingID)
  {
    return BW::BWDATA_Alliance->alliance[this->getID()].player[opposingID];
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  u8 PlayerImpl::getForce()
  {
    return BW::BWDATA_Players->player[this->getID()].nTeam;
  }
  //----------------------------------------------- GET FIRST ------------------------------------------------
  UnitImpl* PlayerImpl::getFirst()
  {
    return UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA_UnitNodeTable_PlayerFirstUnit[getID()]);
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  int PlayerImpl::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit) const
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[this->getID()];
    return 0;
  }
  //------------------------------------------- PLAN TO MAKE -------------------------------------------------
  void PlayerImpl::planToMake(UnitType unit)
  {
    this->toMake[unit.getID()]++;
  }
  //------------------------------------------- GET FORCE NAME -----------------------------------------------
  char* PlayerImpl::getForceName() const
  {
    return BW::BWDATA_ForceNames[BW::BWDATA_Players->player[this->getID()].nTeam].name;
  }
  //----------------------------------------------------------------------------------------------------------
  void PlayerImpl::onGameEnd()
  {
    this->units.clear();
    this->leftTheGame=false;
  }
  bool PlayerImpl::isVictorious() const
  {
    if (this->getID()>=8) return false;
    return BW::BWDATA_PlayerVictory->player[this->getID()]==3;
  }
  bool PlayerImpl::isDefeated() const
  {
    if (this->getID()>=8) return false;
    return BW::BWDATA_PlayerVictory->player[this->getID()]==1 ||
           BW::BWDATA_PlayerVictory->player[this->getID()]==2 ||
           BW::BWDATA_PlayerVictory->player[this->getID()]==4 ||
           BW::BWDATA_PlayerVictory->player[this->getID()]==6;
  }
  //---------------------------------------------- LEFT GAME -------------------------------------------------
  bool PlayerImpl::leftGame() const
  {
    return this->leftTheGame;
  }
  //----------------------------------------------------------------------------------------------------------
};
