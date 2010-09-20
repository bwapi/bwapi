#include "PlayerImpl.h"
#include "GameImpl.h"
#include "UnitImpl.h"

#include <string>
#include <Util/Bitmask.h>

#include <BW/Offsets.h>
#include <BW/UnitID.h>

namespace BWAPI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  PlayerImpl::PlayerImpl(u8 index)
      : index(index), leftTheGame(false)
  {
    id   = -1;
    self = &data;
    for(int i = 55; i < 63; i++)
    {
      self->upgradeLevel[i] = 0;
      self->isUpgrading[i]  = 0;
    }
    for (int i = 228; i < 230; i++)
    {
      self->allUnitCount[i]       = 0;
      self->completedUnitCount[i] = 0;
      self->deadUnitCount[i]      = 0;
      self->killedUnitCount[i]    = 0;
    }
  }
  //--------------------------------------------- DESTRUCTOR -------------------------------------------------
  PlayerImpl::~PlayerImpl()
  {
  }
  //--------------------------------------------- SET ID -----------------------------------------------------
  void PlayerImpl::setID(int newID)
  {
    id = newID;
  }
  //--------------------------------------------- GET INDEX --------------------------------------------------
  int PlayerImpl::getIndex() const
  {
    return index;
  }
  //--------------------------------------------- GET ID -----------------------------------------------------
  int PlayerImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET NAME ---------------------------------------------------
  std::string PlayerImpl::getName() const
  {
    if ( index == 11 )
      return std::string("Neutral");
    return std::string(BW::BWDATA_Players[index].szName);
  }
  //--------------------------------------------- GET UNITS --------------------------------------------------
  const std::set<Unit*>& PlayerImpl::getUnits() const
  {
    return units;
  }
  //--------------------------------------------- GET RACE ---------------------------------------------------
  BWAPI::Race PlayerImpl::getRace() const
  {
    return BWAPI::Race((int)(BW::BWDATA_Players[index].nRace));
  }
  //--------------------------------------------- GET TYPE ---------------------------------------------------
  BWAPI::PlayerType PlayerImpl::getType() const
  {
    return BWAPI::PlayerType((int)(BW::BWDATA_Players[index].nType));
  }
  //--------------------------------------------- GET FORCE --------------------------------------------------
  Force* PlayerImpl::getForce() const
  {
    return (Force*)force;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isAlly(Player* player) const
  {
    if ( !player || isNeutral() || player->isNeutral() || !((PlayerImpl*)player)->isParticipating() )
      return false;
    return BW::BWDATA_Alliance[index].player[ ((PlayerImpl*)player)->getIndex() ] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isEnemy(Player* player) const
  {
    if ( !player || this->isNeutral() || player->isNeutral() || !((PlayerImpl*)player)->isParticipating() )
      return false;
    return BW::BWDATA_Alliance[index].player[ ((PlayerImpl*)player)->getIndex() ] == 0;
  }
  //--------------------------------------------- IS NEUTRAL -------------------------------------------------
  bool PlayerImpl::isNeutral() const
  {
    return index == 11;
  }
  //--------------------------------------------- GET START POSITION -----------------------------------------
  TilePosition PlayerImpl::getStartLocation() const
  {
    /* error checking */
    BroodwarImpl.setLastError(Errors::None);
    if (this->isNeutral())
      return TilePositions::None;
    if ( !BroodwarImpl._isReplay() &&
         BroodwarImpl.self()->isEnemy((Player*)this) &&
         !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return TilePositions::Unknown;
    }
    /* return the start location as a tile position */
    return BWAPI::TilePosition((int)((BW::BWDATA_startPositions[index].x - BW::TILE_SIZE * 2) / BW::TILE_SIZE),
                               (int)((BW::BWDATA_startPositions[index].y - (int)(BW::TILE_SIZE * 1.5)) / BW::TILE_SIZE));
  }
  //--------------------------------------------- IS VICTORIOUS ----------------------------------------------
  bool PlayerImpl::isVictorious() const
  {
    if (index >= 8) 
      return false;
    return BW::BWDATA_PlayerVictory[index] == 3;
  }
  //--------------------------------------------- IS DEFEATED ------------------------------------------------
  bool PlayerImpl::isDefeated() const
  {
    if (index >= 8) 
      return false;
    return BW::BWDATA_PlayerVictory[index] == 1 ||
           BW::BWDATA_PlayerVictory[index] == 2 ||
           BW::BWDATA_PlayerVictory[index] == 4 ||
           BW::BWDATA_PlayerVictory[index] == 6;
  }
  //--------------------------------------------- LEFT GAME --------------------------------------------------
  bool PlayerImpl::leftGame() const
  {
    return this->leftTheGame;
  }
  //--------------------------------------------- MINERALS ---------------------------------------------------
  int PlayerImpl::minerals() const
  {
    return self->minerals;
  }
  //--------------------------------------------- GAS --------------------------------------------------------
  int PlayerImpl::gas() const
  {
    return self->gas;
  }
  //--------------------------------------------- CUMULATIVE MINERALS ----------------------------------------
  int PlayerImpl::cumulativeMinerals() const
  {
    return self->cumulativeMinerals;
  }
  //--------------------------------------------- CUMULATIVE GAS ---------------------------------------------
  int PlayerImpl::cumulativeGas() const
  {
    return self->cumulativeGas;
  }
  //--------------------------------------------- SUPPLY TOTAL -----------------------------------------------
  int PlayerImpl::supplyTotal() const
  {
    return self->supplyTotal[getRace().getID()];
  }
  //--------------------------------------------- SUPPLY USED ------------------------------------------------
  int PlayerImpl::supplyUsed() const
  {
    return self->supplyUsed[getRace().getID()];
  }
  //--------------------------------------------- SUPPLY TOTAL -----------------------------------------------
  int PlayerImpl::supplyTotal(Race race) const
  {
    return self->supplyTotal[race.getID()];
  }
  //--------------------------------------------- SUPPLY USED ------------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    return self->supplyUsed[race.getID()];
  }
  //--------------------------------------------- ALL UNIT COUNT ---------------------------------------------
  int PlayerImpl::allUnitCount(UnitType unit) const
  {
    return self->allUnitCount[unit.getID()];
  }
  //--------------------------------------------- COMPLETED UNIT COUNT ---------------------------------------
  int PlayerImpl::completedUnitCount(UnitType unit) const
  {
    return self->completedUnitCount[unit.getID()];
  }
  //--------------------------------------------- INCOMPLETE UNIT COUNT --------------------------------------
  int PlayerImpl::incompleteUnitCount(UnitType unit) const
  {
    return self->allUnitCount[unit.getID()]-self->completedUnitCount[unit.getID()];
  }
  //--------------------------------------------- DEAD UNIT COUNT --------------------------------------------
  int PlayerImpl::deadUnitCount(UnitType unit) const
  {
    return self->deadUnitCount[unit.getID()];
  }
  //--------------------------------------------- KILLED UNIT COUNT ------------------------------------------
  int PlayerImpl::killedUnitCount(UnitType unit) const
  {
    return self->killedUnitCount[unit.getID()];
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
  //--------------------------------------------- IS RESEARCHING ---------------------------------------------
  bool PlayerImpl::isResearching(TechType tech) const
  {
    return self->isResearching[tech.getID()];
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    return self->isUpgrading[upgrade.getID()];
  }
  //--------------------------------------------- MAX ENERGY -------------------------------------------------
  int PlayerImpl::maxEnergy(UnitType unit) const
  {
    int energy = unit.maxEnergy();
    if ((unit == UnitTypes::Protoss_Arbiter       && getUpgradeLevel(UpgradeTypes::Khaydarin_Core)    > 0) ||
        (unit == UnitTypes::Protoss_Corsair       && getUpgradeLevel(UpgradeTypes::Argus_Jewel)       > 0) ||
        (unit == UnitTypes::Protoss_Dark_Archon   && getUpgradeLevel(UpgradeTypes::Argus_Talisman)    > 0) ||
        (unit == UnitTypes::Protoss_High_Templar  && getUpgradeLevel(UpgradeTypes::Khaydarin_Amulet)  > 0) ||
        (unit == UnitTypes::Terran_Ghost          && getUpgradeLevel(UpgradeTypes::Moebius_Reactor)   > 0) ||
        (unit == UnitTypes::Terran_Battlecruiser  && getUpgradeLevel(UpgradeTypes::Colossus_Reactor)  > 0) ||
        (unit == UnitTypes::Terran_Science_Vessel && getUpgradeLevel(UpgradeTypes::Titan_Reactor)     > 0) ||
        (unit == UnitTypes::Terran_Wraith         && getUpgradeLevel(UpgradeTypes::Apollo_Reactor)    > 0) ||
        (unit == UnitTypes::Terran_Medic          && getUpgradeLevel(UpgradeTypes::Caduceus_Reactor)  > 0) ||
        (unit == UnitTypes::Zerg_Defiler          && getUpgradeLevel(UpgradeTypes::Metasynaptic_Node) > 0) ||
        (unit == UnitTypes::Zerg_Queen            && getUpgradeLevel(UpgradeTypes::Gamete_Meiosis)    > 0) )
      energy += 50;
    return energy;
  }
  //--------------------------------------------- TOP SPEED --------------------------------------------------
  double PlayerImpl::topSpeed(UnitType unit) const
  {
    double speed = unit.topSpeed();
    if ((unit == UnitTypes::Terran_Vulture   && getUpgradeLevel(UpgradeTypes::Ion_Thrusters)        > 0) ||
        (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Pneumatized_Carapace) > 0) ||
        (unit == UnitTypes::Zerg_Zergling    && getUpgradeLevel(UpgradeTypes::Metabolic_Boost)      > 0) ||
        (unit == UnitTypes::Zerg_Hydralisk   && getUpgradeLevel(UpgradeTypes::Muscular_Augments)    > 0) ||
        (unit == UnitTypes::Protoss_Zealot   && getUpgradeLevel(UpgradeTypes::Leg_Enhancements)     > 0) ||
        (unit == UnitTypes::Protoss_Shuttle  && getUpgradeLevel(UpgradeTypes::Gravitic_Drive)       > 0) ||
        (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Gravitic_Boosters)    > 0) ||
        (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Gravitic_Thrusters)   > 0) ||
        (unit == UnitTypes::Zerg_Ultralisk   && getUpgradeLevel(UpgradeTypes::Anabolic_Synthesis)   > 0))
      speed += 0;//?

    return speed;
  }
  //--------------------------------------------- GROUND WEAPON MAX RANGE ------------------------------------
  int PlayerImpl::groundWeaponMaxRange(UnitType unit) const
  {
    int range = unit.groundWeapon().maxRange();
    if ((unit == UnitTypes::Terran_Marine  && getUpgradeLevel(UpgradeTypes::U_238_Shells) > 0) ||
        (unit == UnitTypes::Zerg_Hydralisk && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0))
      range += 1*32;
    if (unit == UnitTypes::Protoss_Dragoon && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0)
      range += 2*32;
    return range;
  }
  //--------------------------------------------- AIR WEAPON MAX RANGE ---------------------------------------
  int PlayerImpl::airWeaponMaxRange(UnitType unit) const
  {
    int range = unit.airWeapon().maxRange();
    if ((unit == UnitTypes::Terran_Marine  && getUpgradeLevel(UpgradeTypes::U_238_Shells) > 0) ||
        (unit == UnitTypes::Zerg_Hydralisk && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0))
      range += 1*32;
    if (unit == UnitTypes::Protoss_Dragoon && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0)
      range += 2*32;
    if (unit == UnitTypes::Terran_Goliath  && getUpgradeLevel(UpgradeTypes::Charon_Boosters) > 0)
      range += 2*32;
    return range;
  }
  //--------------------------------------------- SIGHT RANGE ------------------------------------------------
  int PlayerImpl::sightRange(UnitType unit) const
  {
    int range = unit.sightRange();
    if ((unit == UnitTypes::Terran_Ghost     && getUpgradeLevel(UpgradeTypes::Ocular_Implants) > 0) ||
        (unit == UnitTypes::Zerg_Overlord    && getUpgradeLevel(UpgradeTypes::Antennae)        > 0) ||
        (unit == UnitTypes::Protoss_Observer && getUpgradeLevel(UpgradeTypes::Sensor_Array)    > 0) ||
        (unit == UnitTypes::Protoss_Scout    && getUpgradeLevel(UpgradeTypes::Apial_Sensors)   > 0))
      range += 2*32;
    return range;
  }
  //--------------------------------------------- GROUND WEAPON DAMAGE COOLDOWN ------------------------------
  int PlayerImpl::groundWeaponDamageCooldown(UnitType unit) const
  {
    int cooldown = unit.groundWeapon().damageCooldown();
    if (unit == UnitTypes::Zerg_Zergling && getUpgradeLevel(UpgradeTypes::Adrenal_Glands) > 0)
      cooldown -= 0;//?
    return cooldown;
  }
  //--------------------------------------------- ARMOR ------------------------------------------------------
  int PlayerImpl::armor(UnitType unit) const
  {
    int armor = unit.armor();
    armor += getUpgradeLevel(unit.armorUpgrade());
    if (unit == UnitTypes::Zerg_Ultralisk && getUpgradeLevel(UpgradeTypes::Chitinous_Plating)>0)
      armor += 2;
    return armor;
  }
  //--------------------------------------------- UPDATE -----------------------------------------------------
  void PlayerImpl::updateData()
  { 
    if (this->isNeutral() || (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)))
    {
      self->minerals = 0;
      self->gas      = 0;
      self->cumulativeMinerals = 0;
      self->cumulativeGas      = 0;
      for(int i = 0; i < BW::UPGRADE_TYPE_COUNT; ++i)
        self->upgradeLevel[i]  = 0;
      for(int i = 0; i < BW::TECH_TYPE_COUNT; ++i)
        self->hasResearched[i] = 0;
      for(int i = 0; i < BW::UPGRADE_TYPE_COUNT; ++i)
        self->isUpgrading[i]   = 0;
      for(int i = 0; i < BW::TECH_TYPE_COUNT; ++i)
        self->isResearching[i] = 0;
    }
    else
    {
      self->minerals           = BW::BWDATA_PlayerResources->minerals[index];
      self->gas                = BW::BWDATA_PlayerResources->gas[index];
      self->cumulativeMinerals = BW::BWDATA_PlayerResources->cumulativeMinerals[index];
      self->cumulativeGas      = BW::BWDATA_PlayerResources->cumulativeGas[index];
      for(int i = 0; i < 46; ++i)
        self->upgradeLevel[i] = BW::BWDATA_UpgradeLevelSC->level[index][i];
      for(int i = 46; i < BW::UPGRADE_TYPE_COUNT; ++i)
        self->upgradeLevel[i] = BW::BWDATA_UpgradeLevelBW->level[index][i - 46];
      for(int i = 0; i < 24; ++i)
      {
        if (TechType(i).whatResearches() == UnitTypes::None)
          self->hasResearched[i] = true;
        else
          self->hasResearched[i] = BW::BWDATA_TechResearchSC->enabled[index][i] == 1;
      }
      for(int i = 24; i < BW::TECH_TYPE_COUNT; ++i)
      {
        if (TechType(i).whatResearches() == UnitTypes::None)
          self->hasResearched[i] = true;
        else
          self->hasResearched[i] = BW::BWDATA_TechResearchBW->enabled[index][i - 24] == 1;
      }
      for(int i = 0; i < BW::UPGRADE_TYPE_COUNT; ++i)
        self->isUpgrading[i]   = BW::BWDATA_UpgradeProgress[index].getBit(1 << i);
      for(int i = 0; i < BW::TECH_TYPE_COUNT; ++i)
        self->isResearching[i] = ((Util::BitMask<u64>*) (BW::BWDATA_ResearchProgress + index * 6))->getBit(1 << i);
    }
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      for (u8 i = 0; i < 3; ++i)
      {
        self->supplyTotal[i] = 0;
        self->supplyUsed[i]  = 0;
      }
      for(int i = 0; i < 228; ++i)
      {
        self->allUnitCount[i]       = 0;
        self->completedUnitCount[i] = 0;
        self->deadUnitCount[i]      = 0;
        self->killedUnitCount[i]    = 0;
      }
    }
    else
    {
      for (u8 i = 0; i < 3; ++i)
      {
        self->supplyTotal[i] = BW::BWDATA_Supplies->race[i].available[index];
        if (self->supplyTotal[i] > BW::BWDATA_Supplies->race[i].max[index])
          self->supplyTotal[i] = BW::BWDATA_Supplies->race[i].max[index];
        self->supplyUsed[i] = BW::BWDATA_Supplies->race[i].used[index];
      }
      for(int i = 0; i < 228; ++i)
      {
        self->allUnitCount[i]       = BW::BWDATA_Counts->all[i][index];
        self->completedUnitCount[i] = BW::BWDATA_Counts->completed[i][index];
        self->deadUnitCount[i]      = BW::BWDATA_Counts->dead[i][index];
        self->killedUnitCount[i]    = BW::BWDATA_Counts->killed[i][index];
      }
    }

    if (BW::BWDATA_Players[index].nType == BW::PlayerType::PlayerLeft ||
        BW::BWDATA_Players[index].nType == BW::PlayerType::ComputerLeft ||
       (BW::BWDATA_Players[index].nType == BW::PlayerType::Neutral && !isNeutral()))
    {
      this->leftTheGame = true;
    }
  }
  //--------------------------------------------- GET FORCE --------------------------------------------------
  u8 PlayerImpl::getForce()
  {
    return BW::BWDATA_Players[index].nTeam;
  }
  //--------------------------------------------- GET FORCE NAME ---------------------------------------------
  char* PlayerImpl::getForceName() const
  {
    return BW::BWDATA_ForceNames[BW::BWDATA_Players[index].nTeam].name;
  }
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
  BW::Unit** PlayerImpl::selectedUnit()
  {
    return (BW::Unit**)(BW::BWDATA_PlayerSelection + index * 48);
  }
  //----------------------------------------------------------------------------------------------------------
  void PlayerImpl::onGameEnd()
  {
    this->units.clear();
    this->leftTheGame = false;
  }
  //----------------------------------------------------------------------------------------------------------
  bool PlayerImpl::isParticipating()
  {
    return BW::BWDATA_Supplies->race[BW::Race::Zerg].available[index]    != 0 ||
           BW::BWDATA_Supplies->race[BW::Race::Zerg].used[index]         != 0 ||
           BW::BWDATA_Supplies->race[BW::Race::Terran].available[index]  != 0 ||
           BW::BWDATA_Supplies->race[BW::Race::Terran].used[index]       != 0 ||
           BW::BWDATA_Supplies->race[BW::Race::Protoss].available[index] != 0 ||
           BW::BWDATA_Supplies->race[BW::Race::Protoss].used[index]      != 0;
  }
  //----------------------------------------------------------------------------------------------------------
};
