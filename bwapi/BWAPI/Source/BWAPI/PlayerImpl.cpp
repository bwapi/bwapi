#include "PlayerImpl.h"
#include "GameImpl.h"
#include "UnitImpl.h"

#include <string>
#include <Util/Convenience.h>

#include <BW/Offsets.h>

#include <BWAPI/PlayerType.h>

#include "../../../Debug.h"

namespace BWAPI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  PlayerImpl::PlayerImpl(u8 index)
    : index(index)
  {
    resetResources();
    self->color = index < 12 ? BW::BWDATA::Game.playerColorIndex[index] : Colors::Black;
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
  BWAPI::PlayerType PlayerImpl::getType() const
  {
    return BWAPI::PlayerType((int)(BW::BWDATA::Players[index].nType));
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
    return BW::BWDATA::Game.playerAlliances[index][ static_cast<PlayerImpl*>(player)->getIndex() ] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isEnemy(const Player player) const
  {
    if ( !player || this->isNeutral() || player->isNeutral() || this->isObserver() || player->isObserver() )
      return false;
    return BW::BWDATA::Game.playerAlliances[index][ static_cast<PlayerImpl*>(player)->getIndex() ] == 0;
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

    // Return None if there is no start location
    if (index >= BW::PLAYABLE_PLAYER_COUNT || BW::BWDATA::Game.startPositions[index] == BW::Positions::Origin)
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
    return TilePosition(BW::BWDATA::Game.startPositions[index] - BW::Position((TILE_SIZE * 4) / 2, (TILE_SIZE * 3) / 2));
  }
  //--------------------------------------------- IS VICTORIOUS ----------------------------------------------
  bool PlayerImpl::isVictorious() const
  {
    if ( index >= 8 ) 
      return false;
    return BW::BWDATA::PlayerVictory[index] == 3;
  }
  //--------------------------------------------- IS DEFEATED ------------------------------------------------
  bool PlayerImpl::isDefeated() const
  {
    if ( index >= 8 ) 
      return false;
    return BW::BWDATA::PlayerVictory[index] == 1 ||
           BW::BWDATA::PlayerVictory[index] == 2 ||
           BW::BWDATA::PlayerVictory[index] == 4 ||
           BW::BWDATA::PlayerVictory[index] == 6;
  }
  //--------------------------------------------- UPDATE -----------------------------------------------------
  void PlayerImpl::updateData()
  { 
    self->color = index < BW::PLAYER_COUNT ? BW::BWDATA::Game.playerColorIndex[index] : Colors::Black;
  
    // Get upgrades, tech, resources
    if ( this->isNeutral() || 
      index >= BW::PLAYER_COUNT ||
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

      if (!this->isNeutral() && index < BW::PLAYER_COUNT)
      {
        // set upgrade level for visible enemy units
        for(int i = 0; i < 46; ++i)
        {
          for(UnitType t : UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA::Game.currentUpgradeLevelSC[index][i];
          }
        }
        for (int i = 46; i < BW::UPGRADE_TYPE_COUNT; ++i)
        {
          for(UnitType t : UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA::Game.currentUpgradeLevelBW[index][i - 46];
          }
        }
      }
    }
    else
    {
      this->wasSeenByBWAPIPlayer = true;

      // set resources
      self->minerals           = BW::BWDATA::Game.minerals[index];
      self->gas                = BW::BWDATA::Game.gas[index];
      self->gatheredMinerals   = BW::BWDATA::Game.cumulativeMinerals[index];
      self->gatheredGas        = BW::BWDATA::Game.cumulativeGas[index];
      self->repairedMinerals   = this->_repairedMinerals;
      self->repairedGas        = this->_repairedGas;
      self->refundedMinerals   = this->_refundedMinerals;
      self->refundedGas        = this->_refundedGas;

      // set upgrade level
      for(int i = 0; i < 46; ++i)
      {
        self->upgradeLevel[i]     = BW::BWDATA::Game.currentUpgradeLevelSC[index][i];
        self->maxUpgradeLevel[i]  = BW::BWDATA::Game.maxUpgradeLevelSC[index][i];
      }
      for (int i = 46; i < BW::UPGRADE_TYPE_COUNT; ++i)
      {
        self->upgradeLevel[i]     = BW::BWDATA::Game.currentUpgradeLevelBW[index][i - 46];
        self->maxUpgradeLevel[i]  = BW::BWDATA::Game.maxUpgradeLevelBW[index][i - 46];
      }

      // set abilities researched
      for(int i = 0; i < 24; ++i)
      {
        self->hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA::Game.techResearchedSC[index][i]);
        self->isResearchAvailable[i]  = !!BW::BWDATA::Game.techAvailableSC[index][i];
      }
      for (int i = 24; i < BW::TECH_TYPE_COUNT; ++i)
      {
        self->hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA::Game.techResearchedBW[index][i - 24]);
        self->isResearchAvailable[i]  = !!BW::BWDATA::Game.techAvailableBW[index][i - 24];
      }

      // set upgrades in progress
      for (int i = 0; i < BW::UPGRADE_TYPE_COUNT; ++i)
        self->isUpgrading[i]   = (BW::BWDATA::Game.upgradeInProgressBW[index * 8 + i/8] & (1 << i%8)) != 0;
      
      // set research in progress
      for (int i = 0; i < BW::TECH_TYPE_COUNT; ++i)
        self->isResearching[i] = (BW::BWDATA::Game.techResearchInProgressBW[index * 6 + i/8] & (1 << i%8)) != 0;

      for (int i = 0; i < BW::UNIT_TYPE_COUNT; ++i)
        self->isUnitAvailable[i] = !!BW::BWDATA::Game.unitAvailability[index][i];

      self->hasResearched[TechTypes::Enum::Nuclear_Strike] = self->isUnitAvailable[UnitTypes::Enum::Terran_Nuclear_Missile];
    }

    // Get Scores, supply
    if ( (!BroodwarImpl.isReplay() && 
          BroodwarImpl.self()->isEnemy(this) && 
          !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) ||
          index >= BW::PLAYER_COUNT)
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
        self->supplyTotal[i]  = BW::BWDATA::Game.supplies[i].available[index];
        if (self->supplyTotal[i] > BW::BWDATA::Game.supplies[i].max[index])
          self->supplyTotal[i]  = BW::BWDATA::Game.supplies[i].max[index];
        self->supplyUsed[i]   = BW::BWDATA::Game.supplies[i].used[index];
      }
      // set total unit counts
      for (int i = 0; i < BW::UNIT_TYPE_COUNT; ++i)
      {
        self->deadUnitCount[i]   = BW::BWDATA::Game.unitCounts.dead[i][index];
        self->killedUnitCount[i] = BW::BWDATA::Game.unitCounts.killed[i][index];
      }
      // set macro dead unit counts
      self->deadUnitCount[UnitTypes::AllUnits]    = BW::BWDATA::Game.allUnitsLost[index] + BW::BWDATA::Game.allBuildingsLost[index];
      self->deadUnitCount[UnitTypes::Men]         = BW::BWDATA::Game.allUnitsLost[index];
      self->deadUnitCount[UnitTypes::Buildings]   = BW::BWDATA::Game.allBuildingsLost[index];
      self->deadUnitCount[UnitTypes::Factories]   = BW::BWDATA::Game.allFactoriesLost[index];

      // set macro kill unit counts
      self->killedUnitCount[UnitTypes::AllUnits]  = BW::BWDATA::Game.allUnitsKilled[index] + BW::BWDATA::Game.allBuildingsRazed[index];
      self->killedUnitCount[UnitTypes::Men]       = BW::BWDATA::Game.allUnitsKilled[index];
      self->killedUnitCount[UnitTypes::Buildings] = BW::BWDATA::Game.allBuildingsRazed[index];
      self->killedUnitCount[UnitTypes::Factories] = BW::BWDATA::Game.allFactoriesRazed[index];
      
      // set score counts
      self->totalUnitScore      = BW::BWDATA::Game.allUnitScore[index];
      self->totalKillScore      = BW::BWDATA::Game.allKillScore[index];
      self->totalBuildingScore  = BW::BWDATA::Game.allBuildingScore[index];
      self->totalRazingScore    = BW::BWDATA::Game.allRazingScore[index];
      self->customScore         = BW::BWDATA::Game.customScore[index];
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
    this->clientInfo.clear();
    this->interfaceEvents.clear();

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
};
