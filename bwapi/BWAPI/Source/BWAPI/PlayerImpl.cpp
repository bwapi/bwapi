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
  PlayerImpl::PlayerImpl(u8 index)
      : index(index), leftTheGame(false)
  {
    id = -1;
    self=&data;
    for(int i=55;i<63;i++)
    {
      self->upgradeLevel[i] = 0;
      self->isUpgrading[i]  = 0;
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  PlayerImpl::~PlayerImpl()
  {
  }
  //--------------------------------------------- SET ID -----------------------------------------------------
  void PlayerImpl::setID(int newID)
  {
    id = newID;
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  int PlayerImpl::getID() const
  {
    return id;
  }
  //------------------------------------------------- GET Index -------------------------------------------------
  int PlayerImpl::getIndex() const
  {
    return index;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string PlayerImpl::getName() const
  {
    if (index == 11)
      return std::string("Neutral");
    return std::string(BW::BWDATA_Players->player[index].szName);
  }
  //----------------------------------------------- GET UNITS ------------------------------------------------
  const std::set<Unit*>& PlayerImpl::getUnits() const
  {
    return units;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BWAPI::Race PlayerImpl::getRace() const
  {
    return BWAPI::Race((int)(BW::BWDATA_Players->player[index].nRace));
  }
  //--------------------------------------------- GET TYPE ---------------------------------------------------
  BWAPI::PlayerType PlayerImpl::getType() const
  {
    return BWAPI::PlayerType((int)(BW::BWDATA_Players->player[index].nType));
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  Force* PlayerImpl::getForce() const
  {
    return (Force*)force;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isAlly(Player* player) const
  {
    if (player==NULL || isNeutral() || player->isNeutral()) return false;
    return BW::BWDATA_Alliance->alliance[index].player[((PlayerImpl*)player)->getIndex()] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isEnemy(Player* player) const
  {
    if (player==NULL || this->isNeutral() || player->isNeutral()) return false;
    return BW::BWDATA_Alliance->alliance[index].player[((PlayerImpl*)player)->getIndex()] == 0;
  }
  //----------------------------------------------- IS NEUTRAL -----------------------------------------------
  bool PlayerImpl::isNeutral() const
  {
    return index == 11;
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
    return BWAPI::TilePosition((int)((BW::BWDATA_startPositions[index].x - BW::TILE_SIZE * 2) / BW::TILE_SIZE),
                               (int)((BW::BWDATA_startPositions[index].y - (int)(BW::TILE_SIZE * 1.5)) / BW::TILE_SIZE));
  }
  //------------------------------------------------ MINERALS ------------------------------------------------
  int PlayerImpl::minerals() const
  {
    return self->minerals;
  }
  //-------------------------------------------------- GAS ---------------------------------------------------
  int PlayerImpl::gas() const
  {
    return self->gas;
  }
  //------------------------------------------ CUMULATIVE MINERALS -------------------------------------------
  int PlayerImpl::cumulativeMinerals() const
  {
    return self->cumulativeMinerals;
  }
  //--------------------------------------------- CUMULATIVE GAS ---------------------------------------------
  int PlayerImpl::cumulativeGas() const
  {
    return self->cumulativeGas;
  }
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  int PlayerImpl::supplyTotal() const
  {
    return self->supplyTotal[getRace().getID()];
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int PlayerImpl::supplyUsed() const
  {
    return self->supplyUsed[getRace().getID()];
  }
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  int PlayerImpl::supplyTotal(Race race) const
  {
    return self->supplyTotal[race.getID()];
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    return self->supplyUsed[race.getID()];
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
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
  int PlayerImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    return self->upgradeLevel[upgrade.getID()];
  }
  //--------------------------------------------- HAS RESEARCHED ---------------------------------------------
  bool PlayerImpl::hasResearched(TechType tech) const
  {
    return self->hasResearched[tech.getID()];
  }
  //--------------------------------------------- iS RESEARCHING ---------------------------------------------
  bool PlayerImpl::isResearching(TechType tech) const
  {
    return self->isResearching[tech.getID()];
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    return self->isUpgrading[upgrade.getID()];
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
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
  BW::Unit** PlayerImpl::selectedUnit()
  {
    return (BW::Unit**)(BW::BWDATA_PlayerSelection + index * 48);
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void PlayerImpl::updateData()
  { 
    if (this->isNeutral() || (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)))
    {
      self->minerals = 0;
      self->gas      = 0;
      self->cumulativeMinerals = 0;
      self->cumulativeGas      = 0;
      for(int i=0;i<55;i++)
        self->upgradeLevel[i]  = 0;
      for(int i=0;i<47;i++)
        self->hasResearched[i] = 0;
      for(int i=0;i<55;i++)
        self->isUpgrading[i]   = 0;
      for(int i=0;i<47;i++)
        self->isResearching[i] = 0;
    }
    else
    {
      self->minerals           = BW::BWDATA_PlayerResources->minerals.player[index];
      self->gas                = BW::BWDATA_PlayerResources->gas.player[index];
      self->cumulativeMinerals = BW::BWDATA_PlayerResources->cumulativeMinerals.player[index];
      self->cumulativeGas      = BW::BWDATA_PlayerResources->cumulativeGas.player[index];
      for(int i=0;i<46;i++)
        self->upgradeLevel[i] = (int)(*((u8*)(BW::BWDATA_UpgradeLevelSC + index * 46 + i)));
      for(int i=46;i<55;i++)
        self->upgradeLevel[i] = (int)(*((u8*)(BW::BWDATA_UpgradeLevelBW + index * 15 + i - 46)));
      for(int i=0;i<24;i++)
      {
        if (TechType(i).whatResearches()==UnitTypes::None)
          self->hasResearched[i] = true;
        else
          self->hasResearched[i] = (*((u8*)(BW::BWDATA_TechResearchSC + index * 0x18 + i)) == 1);
      }
      for(int i=24;i<47;i++)
      {
        if (TechType(i).whatResearches()==UnitTypes::None)
          self->hasResearched[i] = true;
        else
          self->hasResearched[i] = (*((u8*)(BW::BWDATA_TechResearchBW + index * 0x14 + i - 0x18)) == 1);
      }
      for(int i=0;i<55;i++)
        self->isUpgrading[i]   = BW::BWDATA_UpgradeProgress->player[index].getBit(1 << i);
      for(int i=0;i<47;i++)
        self->isResearching[i] = ((Util::BitMask<u64>*) (BW::BWDATA_ResearchProgress + index * 6))->getBit(1 << i);
    }
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      for (u8 i = 0; i < 3; i++)
      {
        self->supplyTotal[i] = 0;
        self->supplyUsed[i] = 0;
      }
    }
    else
    {
      for (u8 i = 0; i < 3; i++)
      {
        self->supplyTotal[i] = BW::BWDATA_Supplies->race[i].available.player[index];
        if (self->supplyTotal[i] > BW::BWDATA_Supplies->race[i].max.player[index])
          self->supplyTotal[i] = BW::BWDATA_Supplies->race[i].max.player[index];
        self->supplyUsed[i] = BW::BWDATA_Supplies->race[i].used.player[index];
      }
    }
    if (BW::BWDATA_Players->player[index].nType  == BW::PlayerType::PlayerLeft ||
        BW::BWDATA_Players->player[index].nType  == BW::PlayerType::ComputerLeft ||
       (BW::BWDATA_Players->player[index].nType  == BW::PlayerType::Neutral && !isNeutral()))
    {
      this->leftTheGame = true;
    }
    for (u16 j = 0; j < BW::UNIT_TYPE_COUNT; j++)
      this->toMake[j] = 0;
  }
  //------------------------------------------------ GET ALLIANCE --------------------------------------------
  u8 PlayerImpl::getAlliance(u8 opposingID)
  {
    return BW::BWDATA_Alliance->alliance[index].player[opposingID];
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  u8 PlayerImpl::getForce()
  {
    return BW::BWDATA_Players->player[index].nTeam;
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  int PlayerImpl::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit) const
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[index];
    return 0;
  }
  //------------------------------------------- GET FORCE NAME -----------------------------------------------
  char* PlayerImpl::getForceName() const
  {
    return BW::BWDATA_ForceNames[BW::BWDATA_Players->player[index].nTeam].name;
  }
  //----------------------------------------------------------------------------------------------------------
  void PlayerImpl::onGameEnd()
  {
    this->units.clear();
    this->leftTheGame=false;
  }
  bool PlayerImpl::isVictorious() const
  {
    if (index>=8) return false;
    return BW::BWDATA_PlayerVictory->player[index]==3;
  }
  bool PlayerImpl::isDefeated() const
  {
    if (index>=8) return false;
    return BW::BWDATA_PlayerVictory->player[index]==1 ||
           BW::BWDATA_PlayerVictory->player[index]==2 ||
           BW::BWDATA_PlayerVictory->player[index]==4 ||
           BW::BWDATA_PlayerVictory->player[index]==6;
  }
  //---------------------------------------------- LEFT GAME -------------------------------------------------
  bool PlayerImpl::leftGame() const
  {
    return this->leftTheGame;
  }
  //----------------------------------------------------------------------------------------------------------
};
