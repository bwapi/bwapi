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
    data.color = index < 12 ? BW::BWDATA::Game.playerColorIndex[index] : Colors::Black;
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
    data.color = index < BW::PLAYER_COUNT ? BW::BWDATA::Game.playerColorIndex[index] : Colors::Black;
  
    // Get upgrades, tech, resources
    if ( this->isNeutral() || 
      index >= BW::PLAYER_COUNT ||
         (!BroodwarImpl.isReplay() && 
          BroodwarImpl.self()->isEnemy(this) && 
          !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) )
    {
      data.minerals           = 0;
      data.gas                = 0;
      data.gatheredMinerals   = 0;
      data.gatheredGas        = 0;
      data.repairedMinerals   = 0;
      data.repairedGas        = 0;
      data.refundedMinerals   = 0;
      data.refundedGas        = 0;

      // Reset values
      MemZero(data.upgradeLevel);
      MemZero(data.hasResearched);
      MemZero(data.isUpgrading);
      MemZero(data.isResearching);
    
      MemZero(data.maxUpgradeLevel);
      MemZero(data.isResearchAvailable);
      MemZero(data.isUnitAvailable);

      if (!this->isNeutral() && index < BW::PLAYER_COUNT)
      {
        // set upgrade level for visible enemy units
        for(int i = 0; i < 46; ++i)
        {
          for(UnitType t : UpgradeType(i).whatUses())
          {
            if ( data.completedUnitCount[t] > 0 )
              data.upgradeLevel[i] = BW::BWDATA::Game.currentUpgradeLevelSC[index][i];
          }
        }
        for (int i = 46; i < BW::UPGRADE_TYPE_COUNT; ++i)
        {
          for(UnitType t : UpgradeType(i).whatUses())
          {
            if ( data.completedUnitCount[t] > 0 )
              data.upgradeLevel[i] = BW::BWDATA::Game.currentUpgradeLevelBW[index][i - 46];
          }
        }
      }
    }
    else
    {
      this->wasSeenByBWAPIPlayer = true;

      // set resources
      data.minerals           = BW::BWDATA::Game.minerals[index];
      data.gas                = BW::BWDATA::Game.gas[index];
      data.gatheredMinerals   = BW::BWDATA::Game.cumulativeMinerals[index];
      data.gatheredGas        = BW::BWDATA::Game.cumulativeGas[index];
      data.repairedMinerals   = this->_repairedMinerals;
      data.repairedGas        = this->_repairedGas;
      data.refundedMinerals   = this->_refundedMinerals;
      data.refundedGas        = this->_refundedGas;

      // set upgrade level
      for(int i = 0; i < 46; ++i)
      {
        data.upgradeLevel[i]     = BW::BWDATA::Game.currentUpgradeLevelSC[index][i];
        data.maxUpgradeLevel[i]  = BW::BWDATA::Game.maxUpgradeLevelSC[index][i];
      }
      for (int i = 46; i < BW::UPGRADE_TYPE_COUNT; ++i)
      {
        data.upgradeLevel[i]     = BW::BWDATA::Game.currentUpgradeLevelBW[index][i - 46];
        data.maxUpgradeLevel[i]  = BW::BWDATA::Game.maxUpgradeLevelBW[index][i - 46];
      }

      // set abilities researched
      for(int i = 0; i < 24; ++i)
      {
        data.hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA::Game.techResearchedSC[index][i]);
        data.isResearchAvailable[i]  = !!BW::BWDATA::Game.techAvailableSC[index][i];
      }
      for (int i = 24; i < BW::TECH_TYPE_COUNT; ++i)
      {
        data.hasResearched[i]        = (TechType(i).whatResearches() == UnitTypes::None ? true : !!BW::BWDATA::Game.techResearchedBW[index][i - 24]);
        data.isResearchAvailable[i]  = !!BW::BWDATA::Game.techAvailableBW[index][i - 24];
      }

      // set upgrades in progress
      for (int i = 0; i < BW::UPGRADE_TYPE_COUNT; ++i)
        data.isUpgrading[i]   = (BW::BWDATA::Game.upgradeInProgressBW[index * 8 + i/8] & (1 << i%8)) != 0;
      
      // set research in progress
      for (int i = 0; i < BW::TECH_TYPE_COUNT; ++i)
        data.isResearching[i] = (BW::BWDATA::Game.techResearchInProgressBW[index * 6 + i/8] & (1 << i%8)) != 0;

      for (int i = 0; i < BW::UNIT_TYPE_COUNT; ++i)
        data.isUnitAvailable[i] = !!BW::BWDATA::Game.unitAvailability[index][i];

      data.hasResearched[TechTypes::Enum::Nuclear_Strike] = data.isUnitAvailable[UnitTypes::Enum::Terran_Nuclear_Missile];
    }

    // Get Scores, supply
    if ( (!BroodwarImpl.isReplay() && 
          BroodwarImpl.self()->isEnemy(this) && 
          !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation)) ||
          index >= BW::PLAYER_COUNT)
    {
      MemZero(data.supplyTotal);
      MemZero(data.supplyUsed);
      MemZero(data.deadUnitCount);
      MemZero(data.killedUnitCount);

      data.totalUnitScore      = 0;
      data.totalKillScore      = 0;
      data.totalBuildingScore  = 0;
      data.totalRazingScore    = 0;
      data.customScore         = 0;
    }
    else
    {
      // set supply
      for (u8 i = 0; i < BW::RACE_COUNT; ++i)
      {
        data.supplyTotal[i]  = BW::BWDATA::Game.supplies[i].available[index];
        if (data.supplyTotal[i] > BW::BWDATA::Game.supplies[i].max[index])
          data.supplyTotal[i]  = BW::BWDATA::Game.supplies[i].max[index];
        data.supplyUsed[i]   = BW::BWDATA::Game.supplies[i].used[index];
      }
      // set total unit counts
      for (int i = 0; i < BW::UNIT_TYPE_COUNT; ++i)
      {
        data.deadUnitCount[i]   = BW::BWDATA::Game.unitCounts.dead[i][index];
        data.killedUnitCount[i] = BW::BWDATA::Game.unitCounts.killed[i][index];
      }
      // set macro dead unit counts
      data.deadUnitCount[UnitTypes::AllUnits]    = BW::BWDATA::Game.allUnitsLost[index] + BW::BWDATA::Game.allBuildingsLost[index];
      data.deadUnitCount[UnitTypes::Men]         = BW::BWDATA::Game.allUnitsLost[index];
      data.deadUnitCount[UnitTypes::Buildings]   = BW::BWDATA::Game.allBuildingsLost[index];
      data.deadUnitCount[UnitTypes::Factories]   = BW::BWDATA::Game.allFactoriesLost[index];

      // set macro kill unit counts
      data.killedUnitCount[UnitTypes::AllUnits]  = BW::BWDATA::Game.allUnitsKilled[index] + BW::BWDATA::Game.allBuildingsRazed[index];
      data.killedUnitCount[UnitTypes::Men]       = BW::BWDATA::Game.allUnitsKilled[index];
      data.killedUnitCount[UnitTypes::Buildings] = BW::BWDATA::Game.allBuildingsRazed[index];
      data.killedUnitCount[UnitTypes::Factories] = BW::BWDATA::Game.allFactoriesRazed[index];
      
      // set score counts
      data.totalUnitScore      = BW::BWDATA::Game.allUnitScore[index];
      data.totalKillScore      = BW::BWDATA::Game.allKillScore[index];
      data.totalBuildingScore  = BW::BWDATA::Game.allBuildingScore[index];
      data.totalRazingScore    = BW::BWDATA::Game.allRazingScore[index];
      data.customScore         = BW::BWDATA::Game.customScore[index];
    }

    if (index < BW::PLAYER_COUNT && (BW::BWDATA::Players[index].nType == PlayerTypes::PlayerLeft ||
        BW::BWDATA::Players[index].nType == PlayerTypes::ComputerLeft ||
       (BW::BWDATA::Players[index].nType == PlayerTypes::Neutral && !isNeutral())))
    {
      data.leftGame = true;
    }
  }
  //----------------------------------------------------------------------------------------------------------
  void PlayerImpl::onGameEnd()
  {
    this->units.clear();
    this->clientInfo.clear();
    this->interfaceEvents.clear();

    data.leftGame = false;
    this->wasSeenByBWAPIPlayer = false;
  }
  void PlayerImpl::setParticipating(bool isParticipating)
  {
    data.isParticipating = isParticipating;
  }
  void PlayerImpl::resetResources()
  {
    _repairedMinerals = 0;
    _repairedGas      = 0;
    _refundedMinerals = 0;
    _refundedGas      = 0;
  }
};
