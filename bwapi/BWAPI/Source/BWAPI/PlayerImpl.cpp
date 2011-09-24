#include "PlayerImpl.h"
#include "GameImpl.h"
#include "UnitImpl.h"

#include <string>

#include <BW/Offsets.h>
#include <BW/UnitID.h>
#include <BW/PlayerType.h>

#include "../../Debug.h"

namespace BWAPI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  PlayerImpl::PlayerImpl(u8 index)
      : index(index), leftTheGame(false)
  {
    id   = -1;
    self = &data;
    for(int i = 55; i < 63; ++i)
    {
      self->upgradeLevel[i] = 0;
      self->isUpgrading[i]  = 0;
    }
    for (int i = 228; i < BWAPI_UNIT_TYPE_MAX_COUNT; ++i)
    {
      self->allUnitCount[i]        = 0;
      self->visibleUnitCount[i]    = 0;
      self->completedUnitCount[i]  = 0;
      self->deadUnitCount[i]       = 0;
      self->killedUnitCount[i]     = 0;
    }
    self->totalUnitScore      = 0;
    self->totalKillScore      = 0;
    self->totalBuildingScore  = 0;
    self->totalRazingScore    = 0;
    self->customScore         = 0;
    self->isParticipating     = false;
    self->repairedMinerals    = 0;
    self->repairedGas         = 0;
    self->refundedMinerals    = 0;
    self->refundedGas         = 0;
    resetResources();
    if ( index < 12 )
    {
      self->color = BW::BWDATA_PlayerColors[index];
      switch ( BW::BWDATA_PlayerColors[index] )
      {
      case 111: // red
        self->colorByte = 0x08;
        break;
      case 165: // blue
        self->colorByte = 0x0E;
        break;
      case 159: // teal
        self->colorByte = 0x0F;
        break;
      case 164: // purp
        self->colorByte = 0x10;
        break;
      case 179: // oj
        self->colorByte = 0x11;
        break;
      case 19:  // brown
        self->colorByte = 0x15;
        break;
      case 84:  // white
        self->colorByte = 0x16;
        break;
      case 135: // yellow
        self->colorByte = 0x17;
        break;
      case 185: // green p9
        self->colorByte = 0x18;
        break;
      case 136: // p10
        self->colorByte = 0x19;
        break;
      case 134: // p11
        self->colorByte = 0x1B;
        break;
      case 51:  // p12
        self->colorByte = 0x1C;
        break;
      default:
        self->colorByte = 2;
        break;
      }
    }
    else
    {
      self->color     = 0;
      self->colorByte = 0x02;
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
    if ( !player || this->isNeutral() || player->isNeutral() || this->isObserver() || player->isObserver() )
      return false;
    return BW::BWDATA_Alliance[index].player[ ((PlayerImpl*)player)->getIndex() ] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isEnemy(Player* player) const
  {
    if ( !player || this->isNeutral() || player->isNeutral() || this->isObserver() || player->isObserver() )
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
    if ( this->isNeutral() )
      return TilePositions::None;
    if ( !BroodwarImpl._isReplay() &&
         BroodwarImpl.self()->isEnemy((Player*)this) &&
         !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return TilePositions::Unknown;
    }
    /* return the start location as a tile position */
    return BWAPI::TilePosition((int)((BW::BWDATA_startPositions[index].x - TILE_SIZE * 2) / TILE_SIZE),
                               (int)((BW::BWDATA_startPositions[index].y - (int)(TILE_SIZE * 1.5)) / TILE_SIZE));
  }
  //--------------------------------------------- IS VICTORIOUS ----------------------------------------------
  bool PlayerImpl::isVictorious() const
  {
    if ( index >= 8 ) 
      return false;
    return BW::BWDATA_PlayerVictory[index] == 3;
  }
  //--------------------------------------------- IS DEFEATED ------------------------------------------------
  bool PlayerImpl::isDefeated() const
  {
    if ( index >= 8 ) 
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
  //--------------------------------------------- GATHERED MINERALS ------------------------------------------
  int PlayerImpl::gatheredMinerals() const
  {
    return self->gatheredMinerals;
  }
  //--------------------------------------------- GATHERED GAS -----------------------------------------------
  int PlayerImpl::gatheredGas() const
  {
    return self->gatheredGas;
  }
  //--------------------------------------------- REPAIRED MINERALS ------------------------------------------
  int PlayerImpl::repairedMinerals() const
  {
    return self->repairedMinerals;
  }
  //--------------------------------------------- REPAIRED GAS -----------------------------------------------
  int PlayerImpl::repairedGas() const
  {
    return self->repairedGas;
  }
  //--------------------------------------------- REFUNDED MINERALS ------------------------------------------
  int PlayerImpl::refundedMinerals() const
  {
    return self->refundedMinerals;
  }
  //--------------------------------------------- REFUNDED GAS -----------------------------------------------
  int PlayerImpl::refundedGas() const
  {
    return self->refundedGas;
  }
  //--------------------------------------------- SPENT MINERALS ---------------------------------------------
  int PlayerImpl::spentMinerals() const
  {
    return self->gatheredMinerals + self->refundedMinerals - self->minerals - self->repairedMinerals;
  }
  //--------------------------------------------- SPENT GAS --------------------------------------------------
  int PlayerImpl::spentGas() const
  {
    return self->gatheredGas + self->refundedGas - self->gas - self->repairedGas;
  }
  //--------------------------------------------- SUPPLY TOTAL -----------------------------------------------
  int PlayerImpl::supplyTotal() const
  {
    BWAPI::Race r = getRace();
    if ( r < 0 || r >= 3 )
      return 0;
    return self->supplyTotal[r];
  }
  //--------------------------------------------- SUPPLY USED ------------------------------------------------
  int PlayerImpl::supplyUsed() const
  {
    BWAPI::Race r = getRace();
    if ( r < 0 || r >= 3 )
      return 0;
    return self->supplyUsed[r];
  }
  //--------------------------------------------- SUPPLY TOTAL -----------------------------------------------
  int PlayerImpl::supplyTotal(Race race) const
  {
    if ( race < 0 || race >= 3 )
      return 0;
    return self->supplyTotal[race];
  }
  //--------------------------------------------- SUPPLY USED ------------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    if ( race < 0 || race >= 3 )
      return 0;
    return self->supplyUsed[race];
  }
  //--------------------------------------------- ALL UNIT COUNT ---------------------------------------------
  int PlayerImpl::allUnitCount(UnitType unit) const
  {
    if ( unit < 0 || unit >= BWAPI_UNIT_TYPE_MAX_COUNT )
      return 0;
    return self->allUnitCount[unit];
  }
  //--------------------------------------------- VISIBLE UNIT COUNT -----------------------------------------
  int PlayerImpl::visibleUnitCount(UnitType unit) const
  {
    if ( unit < 0 || unit >= BWAPI_UNIT_TYPE_MAX_COUNT )
      return 0;
    return self->visibleUnitCount[unit];
  }
  //--------------------------------------------- COMPLETED UNIT COUNT ---------------------------------------
  int PlayerImpl::completedUnitCount(UnitType unit) const
  {
    if ( unit < 0 || unit >= BWAPI_UNIT_TYPE_MAX_COUNT )
      return 0;
    return self->completedUnitCount[unit];
  }
  //--------------------------------------------- INCOMPLETE UNIT COUNT --------------------------------------
  int PlayerImpl::incompleteUnitCount(UnitType unit) const
  {
    if ( unit < 0 || unit >= BWAPI_UNIT_TYPE_MAX_COUNT )
      return 0;
    return self->allUnitCount[unit] - self->completedUnitCount[unit];
  }
  //--------------------------------------------- DEAD UNIT COUNT --------------------------------------------
  int PlayerImpl::deadUnitCount(UnitType unit) const
  {
    if ( unit < 0 || unit >= BWAPI_UNIT_TYPE_MAX_COUNT )
      return 0;
    return self->deadUnitCount[unit];
  }
  //--------------------------------------------- KILLED UNIT COUNT ------------------------------------------
  int PlayerImpl::killedUnitCount(UnitType unit) const
  {
    if ( unit < 0 || unit >= BWAPI_UNIT_TYPE_MAX_COUNT )
      return 0;
    return self->killedUnitCount[unit];
  }
  //--------------------------------------------------- SCORE ------------------------------------------------
  int PlayerImpl::getUnitScore() const
  {
    return self->totalUnitScore;
  }
  int PlayerImpl::getKillScore() const
  {
    return self->totalKillScore;
  }
  int PlayerImpl::getBuildingScore() const
  {
    return self->totalBuildingScore;
  }
  int PlayerImpl::getRazingScore() const
  {
    return self->totalRazingScore;
  }
  int PlayerImpl::getCustomScore() const
  {
    return self->customScore;
  }
  //--------------------------------------------- GET UPGRADE LEVEL ------------------------------------------
  int PlayerImpl::getUpgradeLevel(UpgradeType upgrade) const
  {
    if ( upgrade < 0 || upgrade >= 63 )
      return 0;
    return self->upgradeLevel[upgrade];
  }
  //--------------------------------------------- HAS RESEARCHED ---------------------------------------------
  bool PlayerImpl::hasResearched(TechType tech) const
  {
    if ( tech < 0 || tech >= 47 )
      return false;
    return self->hasResearched[tech];
  }
  //--------------------------------------------- IS RESEARCHING ---------------------------------------------
  bool PlayerImpl::isResearching(TechType tech) const
  {
    if ( tech < 0 || tech >= 47 )
      return false;
    return self->isResearching[tech];
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    if ( upgrade < 0 || upgrade >= 63 )
      return false;
    return self->isUpgrading[upgrade];
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
    {
      if ( unit == UnitTypes::Protoss_Scout )
        speed += 427/256.0;
      else
        speed = speed * 1.5;
      if ( speed < 853/256.0 )
        speed = 853/256.0;
      //acceleration *= 2;
      //turnRadius *= 2;
    }
    return speed;
  }
  //--------------------------------------------- GROUND WEAPON MAX RANGE ------------------------------------
  int PlayerImpl::groundWeaponMaxRange(UnitType unit) const
  {
    int range = unit.groundWeapon().maxRange();
    if ( (unit == UnitTypes::Terran_Marine  && getUpgradeLevel(UpgradeTypes::U_238_Shells)   > 0) ||
         (unit == UnitTypes::Zerg_Hydralisk && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0) )
      range += 1*32;
    if (unit == UnitTypes::Protoss_Dragoon && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0)
      range += 2*32;
    return range;
  }
  //--------------------------------------------- AIR WEAPON MAX RANGE ---------------------------------------
  int PlayerImpl::airWeaponMaxRange(UnitType unit) const
  {
    int range = unit.airWeapon().maxRange();
    if ( (unit == UnitTypes::Terran_Marine  && getUpgradeLevel(UpgradeTypes::U_238_Shells)   > 0) ||
         (unit == UnitTypes::Zerg_Hydralisk && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0) )
      range += 1*32;
    if (unit == UnitTypes::Protoss_Dragoon && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0)
      range += 2*32;
    if (unit == UnitTypes::Terran_Goliath  && getUpgradeLevel(UpgradeTypes::Charon_Boosters) > 0)
      range += 3*32;
    return range;
  }
  //----------------------------------------------- WEAPON MAX RANGE -----------------------------------------
  int PlayerImpl::weaponMaxRange(WeaponType weapon) const
  {
    int range = weapon.maxRange();
    if ( (weapon == WeaponTypes::Gauss_Rifle   && getUpgradeLevel(UpgradeTypes::U_238_Shells)   > 0) ||
         (weapon == WeaponTypes::Needle_Spines && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0) )
      range += 1*32;
    else if ( weapon == WeaponTypes::Phase_Disruptor       && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0 )
      range += 2*32;
    else if ( weapon == WeaponTypes::Hellfire_Missile_Pack && getUpgradeLevel(UpgradeTypes::Charon_Boosters)    > 0 )
      range += 3*32;
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
      range = 11*32;
    return range;
  }
  //--------------------------------------------- GROUND WEAPON DAMAGE COOLDOWN ------------------------------
  int PlayerImpl::groundWeaponDamageCooldown(UnitType unit) const
  {
    int cooldown = unit.groundWeapon().damageCooldown();
    if (unit == UnitTypes::Zerg_Zergling && getUpgradeLevel(UpgradeTypes::Adrenal_Glands) > 0)
    {
      cooldown >>= 1;
      if (cooldown >= 250)
        cooldown = 250;
      if (cooldown <= 5)
        cooldown = 5;
    }
    return cooldown;
  }
  //--------------------------------------------- ARMOR ------------------------------------------------------
  int PlayerImpl::armor(UnitType unit) const
  {
    int armor = unit.armor();
    armor += getUpgradeLevel(unit.armorUpgrade());
    if ( unit == UnitTypes::Zerg_Ultralisk && getUpgradeLevel(UpgradeTypes::Chitinous_Plating) > 0 )
      armor += 2;
    else if ( unit == UnitTypes::Hero_Torrasque )
      armor += 2;
    return armor;
  }
  //--------------------------------------------- UPDATE -----------------------------------------------------
  void PlayerImpl::updateData()
  { 
    if ( index < 12 )
    {
      self->color = BW::BWDATA_PlayerColors[index];
      switch ( BW::BWDATA_PlayerColors[index] )
      {
      case 111: // red
        self->colorByte = 0x08;
        break;
      case 165: // blue
        self->colorByte = 0x0E;
        break;
      case 159: // teal
        self->colorByte = 0x0F;
        break;
      case 164: // purp
        self->colorByte = 0x10;
        break;
      case 156: // oj
        self->colorByte = 0x11;
        break;
      case 19:  // brown
        self->colorByte = 0x15;
        break;
      case 84:  // white
        self->colorByte = 0x16;
        break;
      case 135: // yellow
        self->colorByte = 0x17;
        break;
      case 185: // green p9
        self->colorByte = 0x18;
        break;
      case 136: // p10
        self->colorByte = 0x19;
        break;
      case 134: // p11
        self->colorByte = 0x1B;
        break;
      case 51:  // p12
        self->colorByte = 0x1C;
        break;
      default:
        self->colorByte = 2;
        break;
      }
    }
    else
    {
      self->color     = 0;
      self->colorByte = 0x02;
    }

    // Reset values
    MemZero(self->upgradeLevel);
    MemZero(self->hasResearched);
    MemZero(self->isUpgrading);
    MemZero(self->isResearching);
    
    MemZero(self->maxUpgradeLevel);
    MemZero(self->isResearchAvailable);
    MemZero(self->isUnitAvailable);

    if ( this->isNeutral() || 
         (!BroodwarImpl._isReplay() && 
          BroodwarImpl.self()->isEnemy(this) && 
          !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) )
    {
      self->minerals           = 0;
      self->gas                = 0;
      self->gatheredMinerals   = 0;
      self->gatheredGas        = 0;
      self->repairedMinerals   = 0;
      self->repairedGas        = 0;
      self->refundedMinerals   = 0;
      self->refundedGas        = 0;

      if ( !this->isNeutral() && index < 12 )
      {
        // set upgrade level for visible enemy units
        for(int i = 0; i < 46; ++i)
        {
          for each(UnitType t in UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA_UpgradeLevelSC->level[index][i];
          }
        }
        for(int i = 46; i < UPGRADE_TYPE_COUNT; ++i)
        {
          for each(UnitType t in UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA_UpgradeLevelBW->level[index][i - 46];
          }
        }
      }
    }
    else
    {
      // set resources
      self->minerals           = BW::BWDATA_PlayerResources->minerals[index];
      self->gas                = BW::BWDATA_PlayerResources->gas[index];
      self->gatheredMinerals   = BW::BWDATA_PlayerResources->cumulativeMinerals[index];
      self->gatheredGas        = BW::BWDATA_PlayerResources->cumulativeGas[index];
      self->repairedMinerals   = this->_repairedMinerals;
      self->repairedGas        = this->_repairedGas;
      self->refundedMinerals   = this->_refundedMinerals;
      self->refundedGas        = this->_refundedGas;

      // set upgrade level
      for(int i = 0; i < 46; ++i)
      {
        self->upgradeLevel[i]     = BW::BWDATA_UpgradeLevelSC->level[index][i];
        self->maxUpgradeLevel[i]  = BW::BWDATA_UpgradeMaxSC->level[index][i];
      }
      for(int i = 46; i < UPGRADE_TYPE_COUNT; ++i)
      {
        self->upgradeLevel[i]     = BW::BWDATA_UpgradeLevelBW->level[index][i - 46];
        self->maxUpgradeLevel[i]  = BW::BWDATA_UpgradeMaxBW->level[index][i - 46];
      }

      // set abilities researched
      for(int i = 0; i < 24; ++i)
      {
        self->hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA_TechResearchSC->enabled[index][i]);
        self->isResearchAvailable[i]  = !!BW::BWDATA_TechAvailableSC->enabled[index][i];
      }
      for(int i = 24; i < TECH_TYPE_COUNT; ++i)
      {
        self->hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA_TechResearchBW->enabled[index][i - 24]);
        self->isResearchAvailable[i]  = !!BW::BWDATA_TechAvailableBW->enabled[index][i - 24];
      }

      // set upgrades in progress
      for(int i = 0; i < UPGRADE_TYPE_COUNT; ++i)
        self->isUpgrading[i]   = ( *(u8*)(BW::BWDATA_UpgradeProgress + index * 8 + i/8 ) & (1 << i%8)) != 0;
      
      // set research in progress
      for(int i = 0; i < TECH_TYPE_COUNT; ++i)
        self->isResearching[i] = ( *(u8*)(BW::BWDATA_ResearchProgress + index * 6 + i/8 ) & (1 << i%8)) != 0;

      for ( int i = 0; i < UNIT_TYPE_COUNT; ++i )
        self->isUnitAvailable[i] = !!BW::BWDATA_UnitAvailability->available[index][i];

      self->hasResearched[TechTypes::Nuclear_Strike] = self->isUnitAvailable[UnitTypes::Terran_Nuclear_Missile];
    }
    if ( (!BroodwarImpl._isReplay() && 
          BroodwarImpl.self()->isEnemy((Player*)this) && 
          !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) ||
          index >= 12 )
    {
      MemZero(self->supplyTotal);
      MemZero(self->supplyUsed);
      MemZero(self->deadUnitCount);
      MemZero(self->killedUnitCount);

      self->totalUnitScore      = 0;
      self->totalKillScore      = 0;
      self->totalBuildingScore  = 0;
      self->totalRazingScore    = 0;
      self->customScore         = 0;
    }
    else
    {
      // set supply
      for (u8 i = 0; i < RACE_COUNT; ++i)
      {
        self->supplyTotal[i]  = BW::BWDATA_AllScores->supplies[i].available[index];
        if (self->supplyTotal[i] > BW::BWDATA_AllScores->supplies[i].max[index])
          self->supplyTotal[i]  = BW::BWDATA_AllScores->supplies[i].max[index];
        self->supplyUsed[i]   = BW::BWDATA_AllScores->supplies[i].used[index];
      }
      // set total unit counts
      for(int i = 0; i < UNIT_TYPE_COUNT; ++i)
      {
        self->deadUnitCount[i]   = BW::BWDATA_AllScores->unitCounts.dead[i][index];
        self->killedUnitCount[i] = BW::BWDATA_AllScores->unitCounts.killed[i][index];
      }
      // set macro dead unit counts
      self->deadUnitCount[UnitTypes::AllUnits]    = BW::BWDATA_AllScores->allUnitsLost[index] + BW::BWDATA_AllScores->allBuildingsLost[index];
      self->deadUnitCount[UnitTypes::Men]         = BW::BWDATA_AllScores->allUnitsLost[index];
      self->deadUnitCount[UnitTypes::Buildings]   = BW::BWDATA_AllScores->allBuildingsLost[index];
      self->deadUnitCount[UnitTypes::Factories]   = BW::BWDATA_AllScores->allFactoriesLost[index];

      // set macro kill unit counts
      self->killedUnitCount[UnitTypes::AllUnits]  = BW::BWDATA_AllScores->allUnitsKilled[index] + BW::BWDATA_AllScores->allBuildingsRazed[index];
      self->killedUnitCount[UnitTypes::Men]       = BW::BWDATA_AllScores->allUnitsKilled[index];
      self->killedUnitCount[UnitTypes::Buildings] = BW::BWDATA_AllScores->allBuildingsRazed[index];
      self->killedUnitCount[UnitTypes::Factories] = BW::BWDATA_AllScores->allFactoriesRazed[index];
      
      // set score counts
      self->totalUnitScore      = BW::BWDATA_AllScores->allUnitScore[index];
      self->totalKillScore      = BW::BWDATA_AllScores->allKillScore[index];
      self->totalBuildingScore  = BW::BWDATA_AllScores->allBuildingScore[index];
      self->totalRazingScore    = BW::BWDATA_AllScores->allRazingScore[index];
      self->customScore         = BW::BWDATA_AllScores->customScore[index];
    }

    if (BW::BWDATA_Players[index].nType == BW::PlayerType::PlayerLeft ||
        BW::BWDATA_Players[index].nType == BW::PlayerType::ComputerLeft ||
       (BW::BWDATA_Players[index].nType == BW::PlayerType::Neutral && !isNeutral()))
    {
      this->leftTheGame = true;
    }
  }
  //--------------------------------------------- GET FORCE NAME ---------------------------------------------
  char* PlayerImpl::getForceName() const
  {
    u8 team = BW::BWDATA_Players[index].nTeam;
    if ( team == 0 || team > 4 )
      return "";
    team--;
    return BW::BWDATA_ForceNames[team].name;
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
  BWAPI::Color PlayerImpl::getColor() const
  {
    return BWAPI::Color(self->color);
  }
  int PlayerImpl::getTextColor() const
  {
    return self->colorByte;
  }
  //----------------------------------------------------------------------------------------------------------
  bool PlayerImpl::isObserver() const
  {
    return !self->isParticipating;
  }

  void PlayerImpl::setParticipating(bool isParticipating)
  {
    self->isParticipating = isParticipating;
  }
  void PlayerImpl::resetResources()
  {
    _repairedMinerals = 0;
    _repairedGas      = 0;
    _refundedMinerals = 0;
    _refundedGas      = 0;
  }
  //----------------------------------------------------------------------------------------------------------
  int PlayerImpl::getMaxUpgradeLevel(UpgradeType upgrade) const
  {
    if ( upgrade >= UpgradeTypes::None )
      return 0;
    return self->maxUpgradeLevel[upgrade];
  }
  bool PlayerImpl::isResearchAvailable(TechType tech) const
  {
    if ( tech >= TechTypes::None )
      return false;
    return self->isResearchAvailable[tech];
  }
  bool PlayerImpl::isUnitAvailable(UnitType unit) const
  {
    if ( unit >= UnitTypes::None )
      return false;
    return self->isUnitAvailable[unit];
  }
};
