#include "PlayerImpl.h"
#include "GameImpl.h"
#include "UnitImpl.h"

#include <string>
#include "../Convenience.h"

#include "../BW/Offsets.h"

#include <BWAPI/PlayerType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Race.h>
#include <BWAPI/WeaponType.h>

namespace BWAPI4
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  PlayerImpl::PlayerImpl(u8 index)
    : index(index)
  {
    resetResources();
    self->color = index < 12 ? BW::BWDATA::Game.playerColorIndex[index] : Colors::Black.getID();
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
  int PlayerImpl::getNationId() const
  {
    if (index >= BW::BWDATA::playerToNationIdMap.size())
      return 11;
    return BW::BWDATA::playerToNationIdMap[index];
  }
  //--------------------------------------------- GET NAME ---------------------------------------------------
  std::string PlayerImpl::getName() const
  {
    if ( index == 11 )
      return std::string("Neutral");
    return std::string(BW::BWDATA::Players[index].szName);
  }
  //--------------------------------------------- GET RACE ---------------------------------------------------
  BWAPI::Race PlayerImpl::getRace() const
  {
    BroodwarImpl.setLastError();
    if ( this->index < BW::PLAYABLE_PLAYER_COUNT )
    {
      Race rlast = BroodwarImpl.lastKnownRaceBeforeStart[this->index];
      if (  rlast != Races::Zerg          &&
            rlast != Races::Terran        &&
            rlast != Races::Protoss       &&
            !this->wasSeenByBWAPIPlayer   && 
            !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
      {
        BroodwarImpl.setLastError(Errors::Access_Denied);
        return Races::Unknown;
      }
    }
    return BWAPI::Race( BW::BWDATA::Players[index].nRace );
  }
  //--------------------------------------------- GET TYPE ---------------------------------------------------
  BWAPI4::PlayerType PlayerImpl::getType() const
  {
    return BWAPI4::PlayerType((int)(BW::BWDATA::Players[index].nType));
  }
  //--------------------------------------------- GET FORCE --------------------------------------------------
  Force PlayerImpl::getForce() const
  {
    return force;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isAlly(const Player player) const
  {
    if ( !player || this->isNeutral() || player->isNeutral() || this->isObserver() || player->isObserver() )
      return false;
    return BW::BWDATA::Game.playerAlliances[getNationId()][ player->getNationId()] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isEnemy(const Player player) const
  {
    if ( !player || this->isNeutral() || player->isNeutral() || this->isObserver() || player->isObserver() )
      return false;
    return BW::BWDATA::Game.playerAlliances[getNationId()][ player->getNationId() ] == 0;
  }
  //--------------------------------------------- IS NEUTRAL -------------------------------------------------
  bool PlayerImpl::isNeutral() const
  {
    return index == 11;
  }
  //--------------------------------------------- GET START POSITION -----------------------------------------
  TilePosition PlayerImpl::getStartLocation() const
  {
    // Clear last error
    BroodwarImpl.setLastError();
    int nationId = getNationId();

    // Return None if there is no start location
    if (nationId >= BW::PLAYABLE_PLAYER_COUNT || BW::BWDATA::Game.startPositions[nationId] == BW::Positions::Origin)
      return TilePositions::None;

    // Return unknown and set Access_Denied if the start location
    // should not be made available.
    if ( !BroodwarImpl.isReplay() &&
       BroodwarImpl.self()->isEnemy(const_cast<PlayerImpl*>(this)) &&
       !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return TilePositions::Unknown;
    }
    // return the start location as a tile position
    return TilePosition(BW::BWDATA::Game.startPositions[nationId] - BW::Position((BWAPI4::TILEPOSITION_SCALE * 4) / 2, (BWAPI4::TILEPOSITION_SCALE * 3) / 2));
  }
  //--------------------------------------------- IS VICTORIOUS ----------------------------------------------
  bool PlayerImpl::isVictorious() const
  {
    if ( getNationId() >= 8 ) 
      return false;
    return BW::BWDATA::PlayerVictory[getNationId()] == 3;
  }
  //--------------------------------------------- IS DEFEATED ------------------------------------------------
  bool PlayerImpl::isDefeated() const
  {
    if (getNationId() >= 8 )
      return false;
    return BW::BWDATA::PlayerVictory[getNationId()] == 1 ||
           BW::BWDATA::PlayerVictory[getNationId()] == 2 ||
           BW::BWDATA::PlayerVictory[getNationId()] == 4 ||
           BW::BWDATA::PlayerVictory[getNationId()] == 6;
  }
  //--------------------------------------------- UPDATE -----------------------------------------------------
  void PlayerImpl::updateData()
  { 
    self->color = index < BW::PLAYER_COUNT ? BW::BWDATA::Game.playerColorIndex[index] : Colors::Black.getID();
  
    int nationId = getNationId();

    // Get upgrades, tech, resources
    if ( this->isNeutral() || 
      index >= BW::PLAYER_COUNT ||
      nationId >= BW::PLAYER_COUNT ||
         (!BroodwarImpl.isReplay() && 
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

      // Reset values
      MemZero(self->upgradeLevel);
      MemZero(self->hasResearched);
      MemZero(self->isUpgrading);
      MemZero(self->isResearching);
    
      MemZero(self->maxUpgradeLevel);
      MemZero(self->isResearchAvailable);
      MemZero(self->isUnitAvailable);

      if (!this->isNeutral() && index < BW::PLAYER_COUNT && nationId < BW::PLAYER_COUNT)
      {
        // set upgrade level for visible enemy units
        for(int i = 0; i < 46; ++i)
        {
          for(UnitType t : UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA::Game.currentUpgradeLevelSC[nationId][i];
          }
        }
        for (int i = 46; i < BW::UPGRADE_TYPE_COUNT; ++i)
        {
          for(UnitType t : UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA::Game.currentUpgradeLevelBW[nationId][i - 46];
          }
        }
      }
    }
    else
    {
      this->wasSeenByBWAPIPlayer = true;

      // set resources
      self->minerals           = BW::BWDATA::Game.minerals[nationId];
      self->gas                = BW::BWDATA::Game.gas[nationId];
      self->gatheredMinerals   = BW::BWDATA::Game.cumulativeMinerals[nationId];
      self->gatheredGas        = BW::BWDATA::Game.cumulativeGas[nationId];
      self->repairedMinerals   = this->_repairedMinerals;
      self->repairedGas        = this->_repairedGas;
      self->refundedMinerals   = this->_refundedMinerals;
      self->refundedGas        = this->_refundedGas;

      // set upgrade level
      for(int i = 0; i < 46; ++i)
      {
        self->upgradeLevel[i]     = BW::BWDATA::Game.currentUpgradeLevelSC[nationId][i];
        self->maxUpgradeLevel[i]  = BW::BWDATA::Game.maxUpgradeLevelSC[nationId][i];
      }
      for (int i = 46; i < BW::UPGRADE_TYPE_COUNT; ++i)
      {
        self->upgradeLevel[i]     = BW::BWDATA::Game.currentUpgradeLevelBW[nationId][i - 46];
        self->maxUpgradeLevel[i]  = BW::BWDATA::Game.maxUpgradeLevelBW[nationId][i - 46];
      }

      // set abilities researched
      for(int i = 0; i < 24; ++i)
      {
        self->hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA::Game.techResearchedSC[nationId][i]);
        self->isResearchAvailable[i]  = !!BW::BWDATA::Game.techAvailableSC[nationId][i];
      }
      for (int i = 24; i < BW::TECH_TYPE_COUNT; ++i)
      {
        self->hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA::Game.techResearchedBW[nationId][i - 24]);
        self->isResearchAvailable[i]  = !!BW::BWDATA::Game.techAvailableBW[nationId][i - 24];
      }

      // set upgrades in progress
      for (int i = 0; i < BW::UPGRADE_TYPE_COUNT; ++i)
        self->isUpgrading[i]   = (BW::BWDATA::Game.upgradeInProgressBW[nationId * 8 + i/8] & (1 << i%8)) != 0;
      
      // set research in progress
      for (int i = 0; i < BW::TECH_TYPE_COUNT; ++i)
        self->isResearching[i] = (BW::BWDATA::Game.techResearchInProgressBW[nationId * 6 + i/8] & (1 << i%8)) != 0;

      for (int i = 0; i < BW::UNIT_TYPE_COUNT; ++i)
        self->isUnitAvailable[i] = !!BW::BWDATA::Game.unitAvailability[nationId][i];

      self->hasResearched[TechTypes::Enum::Nuclear_Strike] = self->isUnitAvailable[UnitTypes::Enum::Terran_Nuclear_Missile];
    }

    // Get Scores, supply
    if ( (!BroodwarImpl.isReplay() && 
          BroodwarImpl.self()->isEnemy(this) && 
          !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) ||
          index >= BW::PLAYER_COUNT ||
          nationId >= BW::PLAYER_COUNT)
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
      for (u8 i = 0; i < BW::RACE_COUNT; ++i)
      {
        self->supplyTotal[i]  = BW::BWDATA::Game.supplies[i].available[nationId];
        if (self->supplyTotal[i] > BW::BWDATA::Game.supplies[i].max[nationId])
          self->supplyTotal[i]  = BW::BWDATA::Game.supplies[i].max[nationId];
        self->supplyUsed[i]   = BW::BWDATA::Game.supplies[i].used[nationId];
      }
      // set total unit counts
      for (int i = 0; i < BW::UNIT_TYPE_COUNT; ++i)
      {
        self->deadUnitCount[i]   = BW::BWDATA::Game.unitCounts.dead[i][nationId];
        self->killedUnitCount[i] = BW::BWDATA::Game.unitCounts.killed[i][nationId];
      }
      // set macro dead unit counts
      self->deadUnitCount[UnitTypes::AllUnits]    = BW::BWDATA::Game.allUnitsLost[nationId] + BW::BWDATA::Game.allBuildingsLost[nationId];
      self->deadUnitCount[UnitTypes::Men]         = BW::BWDATA::Game.allUnitsLost[nationId];
      self->deadUnitCount[UnitTypes::Buildings]   = BW::BWDATA::Game.allBuildingsLost[nationId];
      self->deadUnitCount[UnitTypes::Factories]   = BW::BWDATA::Game.allFactoriesLost[nationId];

      // set macro kill unit counts
      self->killedUnitCount[UnitTypes::AllUnits]  = BW::BWDATA::Game.allUnitsKilled[nationId] + BW::BWDATA::Game.allBuildingsRazed[nationId];
      self->killedUnitCount[UnitTypes::Men]       = BW::BWDATA::Game.allUnitsKilled[nationId];
      self->killedUnitCount[UnitTypes::Buildings] = BW::BWDATA::Game.allBuildingsRazed[nationId];
      self->killedUnitCount[UnitTypes::Factories] = BW::BWDATA::Game.allFactoriesRazed[nationId];
      
      // set score counts
      self->totalUnitScore      = BW::BWDATA::Game.allUnitScore[nationId];
      self->totalKillScore      = BW::BWDATA::Game.allKillScore[nationId];
      self->totalBuildingScore  = BW::BWDATA::Game.allBuildingScore[nationId];
      self->totalRazingScore    = BW::BWDATA::Game.allRazingScore[nationId];
      self->customScore         = BW::BWDATA::Game.customScore[nationId];
    }

    if (index < BW::PLAYER_COUNT && (BW::BWDATA::Players[index].nType == PlayerTypes::PlayerLeft ||
        BW::BWDATA::Players[index].nType == PlayerTypes::ComputerLeft ||
       (BW::BWDATA::Players[index].nType == PlayerTypes::Neutral && !isNeutral())))
    {
      self->leftGame = true;
    }
  }
  //----------------------------------------------------------------------------------------------------------
  void PlayerImpl::onGameEnd()
  {
    this->units.clear();

    self->leftGame = false;
    this->wasSeenByBWAPIPlayer = false;
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
  //--------------------------------------------- GET ID -----------------------------------------------------
  int PlayerImpl::getID() const
  {
    return id;
  }
  //--------------------------------------------- GET UNITS --------------------------------------------------
  const Unitset &PlayerImpl::getUnits() const
  {
    return this->units;
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
  int PlayerImpl::supplyTotal(Race race) const
  {
    if (race == Races::None)  // Get current race's supply if None is specified
      race = this->getRace();

    if (static_cast<unsigned>(race) < std::extent<decltype(self->supplyTotal)>::value)
      return self->supplyTotal[race];
    return 0;
  }
  //--------------------------------------------- SUPPLY USED ------------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    if (race == Races::None)  // Get current race's supply if None is specified
      race = this->getRace();

    if (static_cast<unsigned>(race) < std::extent<decltype(self->supplyUsed)>::value)
      return self->supplyUsed[race];
    return 0;
  }
  //--------------------------------------------- ALL UNIT COUNT ---------------------------------------------
  int PlayerImpl::allUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? self->allUnitCount[unit] : 0;
  }
  //--------------------------------------------- VISIBLE UNIT COUNT -----------------------------------------
  int PlayerImpl::visibleUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? self->visibleUnitCount[unit] : 0;
  }
  //--------------------------------------------- COMPLETED UNIT COUNT ---------------------------------------
  int PlayerImpl::completedUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? self->completedUnitCount[unit] : 0;
  }
  //--------------------------------------------- DEAD UNIT COUNT --------------------------------------------
  int PlayerImpl::deadUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? self->deadUnitCount[unit] : 0;
  }
  //--------------------------------------------- KILLED UNIT COUNT ------------------------------------------
  int PlayerImpl::killedUnitCount(UnitType unit) const
  {
    return (unit >= 0 && unit < UnitTypes::Enum::MAX) ? self->killedUnitCount[unit] : 0;
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
    return upgrade.isValid() ? self->upgradeLevel[upgrade] : 0;
  }
  //--------------------------------------------- HAS RESEARCHED ---------------------------------------------
  bool PlayerImpl::hasResearched(TechType tech) const
  {
    return tech.isValid() ? self->hasResearched[tech] : false;
  }
  //--------------------------------------------- IS RESEARCHING ---------------------------------------------
  bool PlayerImpl::isResearching(TechType tech) const
  {
    return tech.isValid() ? self->isResearching[tech] : false;
  }
  //--------------------------------------------- IS UPGRADING -----------------------------------------------
  bool PlayerImpl::isUpgrading(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? self->isUpgrading[upgrade] : false;
  }
  //-------------------------------------------------- COLOUR ------------------------------------------------
  BWAPI4::Color PlayerImpl::getColor() const
  {
    return BWAPI4::Color(self->color);
  }
  //------------------------------------------------- OBSERVER -----------------------------------------------
  bool PlayerImpl::isObserver() const
  {
    return !self->isParticipating;
  }
  //------------------------------------------------ AVAILABILITY --------------------------------------------
  int PlayerImpl::getMaxUpgradeLevel(UpgradeType upgrade) const
  {
    return upgrade.isValid() ? self->maxUpgradeLevel[upgrade] : 0;
  }
  bool PlayerImpl::isResearchAvailable(TechType tech) const
  {
    return tech.isValid() ? self->isResearchAvailable[tech] : false;
  }
  bool PlayerImpl::isUnitAvailable(UnitType unit) const
  {
    return unit.isValid() ? self->isUnitAvailable[unit] : false;
  }
  //--------------------------------------------- LEFT GAME --------------------------------------------------
  bool PlayerImpl::leftGame() const
  {
    return self->leftGame;
  }

  //-------------------------------------------- TEXT COLOR --------------------------------------------------
  char PlayerImpl::getTextColor() const
  {
    switch (this->getColor())
    {
    case 111: // red
      return Text::BrightRed;
    case 165: // blue
      return Text::Blue;
    case 159: // teal
      return Text::Teal;
    case 164: // purp
      return Text::Purple;
    case 156: // oj
      return Text::Orange;
    case 19:  // brown
      return Text::Brown;
    case 84:  // white
      return Text::PlayerWhite;
    case 135: // yellow
      return Text::PlayerYellow;
    case 185: // green p9
      return Text::DarkGreen;
    case 136: // p10
      return Text::LightYellow;
    case 134: // p11
      return Text::Tan;
    case 51:  // p12
      return Text::GreyBlue;
    default:
      return Text::Default;
    }
  }
  //-------------------------------------- UNIT TYPE REQUIREMENT ---------------------------------------------
  bool PlayerImpl::hasUnitTypeRequirement(UnitType unit, int amount) const
  {
    if (unit == UnitTypes::None)
      return true;

    switch (unit)
    {
    case UnitTypes::Enum::Zerg_Hatchery:
      return completedUnitCount(UnitTypes::Zerg_Hatchery) + allUnitCount(UnitTypes::Zerg_Lair) + allUnitCount(UnitTypes::Zerg_Hive) >= amount;
    case UnitTypes::Enum::Zerg_Lair:
      return completedUnitCount(UnitTypes::Zerg_Lair) + allUnitCount(UnitTypes::Zerg_Hive) >= amount;
    case UnitTypes::Enum::Zerg_Spire:
      return completedUnitCount(UnitTypes::Zerg_Spire) + allUnitCount(UnitTypes::Zerg_Greater_Spire) >= amount;
    default:
      return completedUnitCount(unit) >= amount;
    }
  }

  int PlayerImpl::weaponMaxRange(WeaponType weapon) const
  {
    int range = weapon.maxRange();
    if ((weapon == WeaponTypes::Gauss_Rifle   && getUpgradeLevel(UpgradeTypes::U_238_Shells) > 0) ||
      (weapon == WeaponTypes::Needle_Spines && getUpgradeLevel(UpgradeTypes::Grooved_Spines) > 0))
      range += 1 * 32;
    else if (weapon == WeaponTypes::Phase_Disruptor       && getUpgradeLevel(UpgradeTypes::Singularity_Charge) > 0)
      range += 2 * 32;
    else if (weapon == WeaponTypes::Hellfire_Missile_Pack && getUpgradeLevel(UpgradeTypes::Charon_Boosters) > 0)
      range += 3 * 32;
    return range;
  }

};
