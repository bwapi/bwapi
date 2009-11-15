#include "PlayerImpl.h"
#include "GameImpl.h"
#include "UnitImpl.h"
#include "Globals.h"

#include <string>
#include <Util/Bitmask.h>

#include <BW/Offsets.h>
#include <BW/UnitID.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  PlayerImpl::PlayerImpl(u8 id)
      : id(id)
      , unitCacheFrame(-1)
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
    return std::string(BW::BWDATA_Players->player[this->getID()].name);
  }
  //----------------------------------------------- GET UNITS ------------------------------------------------
  std::set<Unit*> PlayerImpl::getUnits()
  {
    /* Get all the units that this player owns */
    BroodwarImpl.setLastError(Errors::None);
    if (this->unitCacheFrame != BWAPI::BroodwarImpl.getFrameCount())
    {
      this->units.clear();
      for each (UnitImpl* u in BWAPI::BroodwarImpl.units)
        if (u->getPlayer() == this && u->canAccess())
          this->units.insert((Unit*)u);
      this->unitCacheFrame = BWAPI::BroodwarImpl.getFrameCount();
    }
    return this->units;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BWAPI::Race PlayerImpl::getRace() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BWAPI::Race((int)(BW::BWDATA_Players->player[this->getID()].race));
  }
  //----------------------------------------------- PLAYER TYPE ----------------------------------------------
  BWAPI::PlayerType PlayerImpl::playerType() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BWAPI::PlayerType((int)(BW::BWDATA_Players->player[this->getID()].type));
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
    return BW::BWDATA_Alliance->alliance[this->getID()].player[((PlayerImpl*)player)->getID()] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool PlayerImpl::isEnemy(Player* player) const
  {
    BroodwarImpl.setLastError(Errors::None);
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
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
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
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
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
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
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
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
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
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
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
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    s32 ret = this->suppliesAvailableLocal[static_cast<BW::Race::Enum>(getRace().getID())];
    return ret < getSuppliesMaxSync(static_cast<BW::Race::Enum>(getRace().getID())) ? ret : getSuppliesMaxSync(static_cast<BW::Race::Enum>(getRace().getID()));
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int PlayerImpl::supplyUsed() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
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
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    s32 ret = this->suppliesAvailableLocal[static_cast<BW::Race::Enum>(race.getID())];
    return ret < getSuppliesMaxSync(static_cast<BW::Race::Enum>(race.getID())) ? ret : getSuppliesMaxSync(static_cast<BW::Race::Enum>(race.getID()));
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int PlayerImpl::supplyUsed(Race race) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl.inReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->suppliesUsedLocal[race.getID()];
  }
  //--------------------------------------------- GET ALL UNITS ----------------------------------------------
  s32 PlayerImpl::allUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->all, BW::UnitType(BW::UnitID::Enum(unit.getID()))) + this->toMake[unit.getID()];
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 PlayerImpl::completedUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->completed, BW::UnitType(BW::UnitID::Enum(unit.getID())));
  }
  //------------------------------------------ GET INCOMPLETE UNITS ------------------------------------------
  s32 PlayerImpl::incompleteUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->allUnitCount(unit) - this->completedUnitCount(unit) + toMake[unit.getID()];
  }
  //----------------------------------------------- GET DEATHS -----------------------------------------------
  s32 PlayerImpl::deadUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->dead, BW::UnitType(BW::UnitID::Enum(unit.getID())));
  }
  //----------------------------------------------- GET KILLS ------------------------------------------------
  s32 PlayerImpl::killedUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->killed, BW::UnitType(BW::UnitID::Enum(unit.getID())));
  }
  //------------------------------------------ RESEARCH IN PROGRESS ------------------------------------------
  bool PlayerImpl::researching(BWAPI::TechType tech) const
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
  bool PlayerImpl::researched(BWAPI::TechType tech) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return false;
    }
    if (tech.getID() < 0x18)
      return *((u8*)(BW::BWDATA_TechResearchSC + this->getID() * 0x18 + tech.getID())) == 1;
    else
      return *((u8*)(BW::BWDATA_TechResearchBW + this->getID() * 0x14 + tech.getID() - 0x18)) == 1;
  }
  //------------------------------------------ UPGRADE IN PROGRESS -------------------------------------------
  bool PlayerImpl::upgrading(UpgradeType upgrade) const
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
  int PlayerImpl::upgradeLevel(UpgradeType upgrade) const
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


  //---------------------------------------------- GET MINERALS ----------------------------------------------
  s32 PlayerImpl::getMineralsSync() const
  {
    return BW::BWDATA_PlayerResources->minerals.player[this->getID()];
  }
  //------------------------------------------------ GET GAS -------------------------------------------------
  s32 PlayerImpl::getGasSync() const
  {
    return BW::BWDATA_PlayerResources->gas.player[this->getID()];
  }
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
#pragma warning(push)
#pragma warning(disable:4312)
  BW::Unit** PlayerImpl::selectedUnit()
  {
    return (BW::Unit**)(BW::BWDATA_PlayerSelection + this->getID() * 48);
  }
#pragma warning(pop)
  //------------------------------------------------- UPDATE -------------------------------------------------
  void PlayerImpl::update()
  {
    this->mineralsLocal = this->getMineralsSync();
    this->gasLocal = this->getGasSync();
    for (int i = 0; i < BW::RACE_COUNT; i++)
    {
      this->suppliesAvailableLocal[i] = this->getSuppliesAvailableSync((BW::Race::Enum)i);
      this->suppliesUsedLocal[i] = this->getSuppliesUsedSync((BW::Race::Enum)i);
    }
    for (u16 j = 0; j < BW::UNIT_TYPE_COUNT; j++)
      this->toMake[j] = 0;
  }
  //---------------------------------------------- SPEND LOCAL -----------------------------------------------
  void  PlayerImpl::spend(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //------------------------------------------ GET SUPPLY AVAILABLE ------------------------------------------
  s32 PlayerImpl::getSuppliesAvailableSync(BW::Race::Enum race) const
  {
    s32 ret = BW::BWDATA_Supplies->race[race].available.player[this->getID()];
    return ret < getSuppliesMaxSync(race) ? ret : getSuppliesMaxSync(race);
  }
  //-------------------------------------------- GET SUPPLY USED ---------------------------------------------
  s32 PlayerImpl::getSuppliesUsedSync(BW::Race::Enum race) const
  {
    return BW::BWDATA_Supplies->race[race].used.player[this->getID()];
  }
  //--------------------------------------------- GET SUPPLY MAX ---------------------------------------------
  s32 PlayerImpl::getSuppliesMaxSync(BW::Race::Enum race) const
  {
    return BW::BWDATA_Supplies->race[race].max.player[this->getID()];
  }
  //--------------------------------------- USE SUPPLIES PROTOSS LOCAL ---------------------------------------
  void PlayerImpl::useSupplies(u8 supplies, BW::Race::Enum race)
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
    return BW::BWDATA_Players->player[this->getID()].force;
  }
  //----------------------------------------------- GET FIRST ------------------------------------------------
  UnitImpl* PlayerImpl::getFirst()
  {
    return UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA_UnitNodeTable_PlayerFirstUnit[getID()]);
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  s32 PlayerImpl::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit) const
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[this->getID()];
    return 0;
  }
  //------------------------------------------- PLAN TO MAKE -------------------------------------------------
  void PlayerImpl::planToMake(BW::UnitType unit)
  {
    this->toMake[unit.getID()]++;
  }
  //------------------------------------------- GET FORCE NAME -----------------------------------------------
  char* PlayerImpl::getForceName() const
  {
    return BW::BWDATA_ForceNames[BW::BWDATA_Players->player[this->getID()].force].name;
  }
  //----------------------------------------------------------------------------------------------------------
  void PlayerImpl::onGameEnd()
  {
    this->unitCacheFrame=-1;
    this->units.clear();
  }
  //----------------------------------------------------------------------------------------------------------
};
