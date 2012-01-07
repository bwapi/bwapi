#include "PlayerImpl.h"
#include "GameImpl.h"
#include "UnitImpl.h"

#include <string>

#include <BW/Offsets.h>
#include <BW/UnitID.h>
#include <BW/PlayerType.h>

#include <Util/Foreach.h>

#include "../../Debug.h"

namespace BWAPI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  PlayerImpl::PlayerImpl(u8 index)
      : index(index)
      , id(-1)
      , self(&data)
      , force(NULL)
      , wasSeenByBWAPIPlayer(false)
  {
    MemZero(data);
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
  //--------------------------------------------- GET NAME ---------------------------------------------------
  std::string PlayerImpl::getName() const
  {
    if ( index == 11 )
      return std::string("Neutral");
    return std::string(BW::BWDATA_Players[index].szName);
  }
  //--------------------------------------------- GET RACE ---------------------------------------------------
  BWAPI::Race PlayerImpl::getRace() const
  {
    BroodwarImpl.setLastError(Errors::None);
    /*Race rlast = BroodwarImpl.lastKnownRaceBeforeStart[this->index];
    if (  rlast != Races::Zerg    &&
          rlast != Races::Terran  &&
          rlast != Races::Protoss &&
          !wasSeenByBWAPIPlayer   && 
          !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation) )
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return Races::Unknown;
    }*/
    //@TODO INCORRECT with our Race type values
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
          foreach(UnitType t, UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA_UpgradeLevelSC->level[index][i];
          }
        }
        for(int i = 46; i < UPGRADE_TYPE_COUNT; ++i)
        {
          foreach(UnitType t, UpgradeType(i).whatUses())
          {
            if ( self->completedUnitCount[t] > 0 )
              self->upgradeLevel[i] = BW::BWDATA_UpgradeLevelBW->level[index][i - 46];
          }
        }
      }
    }
    else
    {
      this->wasSeenByBWAPIPlayer = true;

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
      self->leftGame = true;
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
