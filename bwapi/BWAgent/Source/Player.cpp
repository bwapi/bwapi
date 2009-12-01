#include "Player.h"
#include "GameImpl.h"
#include "UnitImpl.h"

#include <string>
#include <Util/Bitmask.h>

#include <BW/Offsets.h>
#include <BW/UnitID.h>

namespace BWAgent
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Player::Player(u8 id)
      : id(id), leftTheGame(false)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Player::~Player()
  {
  }
  //------------------------------------------------- GET ID -------------------------------------------------
  int Player::getID() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return this->id;
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string Player::getName() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this->getID() == 11)
    {
      return std::string("Neutral");
    }
    return std::string(BW::BWDATA_Players->player[this->getID()].name);
  }
  //----------------------------------------------- GET UNITS ------------------------------------------------
  const std::set<Unit*>& Player::getUnits() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return this->units;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BWAPI::Race Player::getRace() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BWAPI::Race((int)(BW::BWDATA_Players->player[this->getID()].race));
  }
  //----------------------------------------------- PLAYER TYPE ----------------------------------------------
  BWAPI::PlayerType Player::playerType() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BWAPI::PlayerType((int)(BW::BWDATA_Players->player[this->getID()].type));
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  Force* Player::getForce() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return (Force*)this->force;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool Player::isAlly(Player* player) const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BW::BWDATA_Alliance->alliance[this->getID()].player[((Player*)player)->getID()] != 0;
  }
  //--------------------------------------------- IS ALLIES WITH ---------------------------------------------
  bool Player::isEnemy(Player* player) const
  {
    BroodwarImpl.setLastError(Errors::None);
    return BW::BWDATA_Alliance->alliance[this->getID()].player[((Player*)player)->getID()] == 0;
  }
  //----------------------------------------------- IS NEUTRAL -----------------------------------------------
  bool Player::isNeutral() const
  {
    BroodwarImpl.setLastError(Errors::None);
    return this->getID() == 11;
  }
  //------------------------------------------- GET START POSITION -------------------------------------------
  TilePosition Player::getStartLocation() const
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
  int Player::minerals() const
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
  int Player::gas() const
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
  int Player::cumulativeMinerals() const
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
  int Player::cumulativeGas() const
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
  int Player::supplyTotal() const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    s32 ret = this->suppliesAvailableLocal[getRace().getID()];
    return ret < getSuppliesMaxSync((u8)getRace().getID()) ? ret : getSuppliesMaxSync((u8)getRace().getID());
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int Player::supplyUsed() const
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
  int Player::supplyTotal(Race race) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (!BroodwarImpl._isReplay() && BroodwarImpl.self()->isEnemy((Player*)this) && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    s32 ret = this->suppliesAvailableLocal[race.getID()];
    return ret < getSuppliesMaxSync((u8)race.getID()) ? ret : getSuppliesMaxSync((u8)race.getID());
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  int Player::supplyUsed(Race race) const
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
  s32 Player::allUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->all, BW::UnitType((u16)unit.getID())) + this->toMake[unit.getID()];
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 Player::completedUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->completed, BW::UnitType((u16)unit.getID()));
  }
  //------------------------------------------ GET INCOMPLETE UNITS ------------------------------------------
  s32 Player::incompleteUnitCount(UnitType unit) const
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
  s32 Player::deadUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->dead, BW::UnitType((u16)unit.getID()));
  }
  //----------------------------------------------- GET KILLS ------------------------------------------------
  s32 Player::killedUnitCount(UnitType unit) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return 0;
    }
    return this->evaluateCounts(BW::BWDATA_Counts->killed, BW::UnitType((u16)unit.getID()));
  }
  //------------------------------------------ RESEARCH IN PROGRESS ------------------------------------------
  bool Player::isResearching(BWAPI::TechType tech) const
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
  bool Player::hasResearched(BWAPI::TechType tech) const
  {
    BroodwarImpl.setLastError(Errors::None);
    if (this!=BroodwarImpl.self() && !BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
    {
      BroodwarImpl.setLastError(Errors::Access_Denied);
      return false;
    }
    if (*tech.whatResearches()==UnitTypes::None)
    {
      return true;
    }
    if (tech.getID() < 0x18)
      return *((u8*)(BW::BWDATA_TechResearchSC + this->getID() * 0x18 + tech.getID())) == 1;
    else
      return *((u8*)(BW::BWDATA_TechResearchBW + this->getID() * 0x14 + tech.getID() - 0x18)) == 1;
  }
  //------------------------------------------ UPGRADE IN PROGRESS -------------------------------------------
  bool Player::isUpgrading(UpgradeType upgrade) const
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
  int Player::getUpgradeLevel(UpgradeType upgrade) const
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
  s32 Player::getMineralsSync() const
  {
    return BW::BWDATA_PlayerResources->minerals.player[this->getID()];
  }
  //------------------------------------------------ GET GAS -------------------------------------------------
  s32 Player::getGasSync() const
  {
    return BW::BWDATA_PlayerResources->gas.player[this->getID()];
  }
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
  BW::Unit** Player::selectedUnit()
  {
    return (BW::Unit**)(BW::BWDATA_PlayerSelection + this->getID() * 48);
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void Player::update()
  {
    this->mineralsLocal = this->getMineralsSync();
    this->gasLocal = this->getGasSync();
    for (u8 i = 0; i < BW::RACE_COUNT; i++)
    {
      this->suppliesAvailableLocal[i] = this->getSuppliesAvailableSync(i);
      this->suppliesUsedLocal[i] = this->getSuppliesUsedSync(i);
    }
    if (BW::BWDATA_Players->player[this->getID()].type  == BW::PlayerType::HumanDefeated ||
        BW::BWDATA_Players->player[this->getID()].type  == BW::PlayerType::Computer ||
        (BW::BWDATA_Players->player[this->getID()].type == BW::PlayerType::Neutral && !this->isNeutral()))
    {
      this->leftTheGame = true;
    }
    for (u16 j = 0; j < BW::UNIT_TYPE_COUNT; j++)
      this->toMake[j] = 0;
  }
  //---------------------------------------------- SPEND LOCAL -----------------------------------------------
  void  Player::spend(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //------------------------------------------ GET SUPPLY AVAILABLE ------------------------------------------
  s32 Player::getSuppliesAvailableSync(u8 race) const
  {
    s32 ret = BW::BWDATA_Supplies->race[race].available.player[this->getID()];
    return ret < getSuppliesMaxSync(race) ? ret : getSuppliesMaxSync(race);
  }
  //-------------------------------------------- GET SUPPLY USED ---------------------------------------------
  s32 Player::getSuppliesUsedSync(u8 race) const
  {
    return BW::BWDATA_Supplies->race[race].used.player[this->getID()];
  }
  //--------------------------------------------- GET SUPPLY MAX ---------------------------------------------
  s32 Player::getSuppliesMaxSync(u8 race) const
  {
    return BW::BWDATA_Supplies->race[race].max.player[this->getID()];
  }
  //--------------------------------------- USE SUPPLIES PROTOSS LOCAL ---------------------------------------
  void Player::useSupplies(u8 supplies, u8 race)
  {
    this->suppliesUsedLocal[race] += supplies;
  }
  //------------------------------------------------ GET ALLIANCE --------------------------------------------
  u8 Player::getAlliance(u8 opposingID)
  {
    return BW::BWDATA_Alliance->alliance[this->getID()].player[opposingID];
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  u8 Player::getForce()
  {
    return BW::BWDATA_Players->player[this->getID()].force;
  }
  //----------------------------------------------- GET FIRST ------------------------------------------------
  UnitImpl* Player::getFirst()
  {
    return UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA_UnitNodeTable_PlayerFirstUnit[getID()]);
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  s32 Player::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit) const
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[this->getID()];
    return 0;
  }
  //------------------------------------------- PLAN TO MAKE -------------------------------------------------
  void Player::planToMake(BW::UnitType unit)
  {
    this->toMake[unit.getID()]++;
  }
  //------------------------------------------- GET FORCE NAME -----------------------------------------------
  char* Player::getForceName() const
  {
    return BW::BWDATA_ForceNames[BW::BWDATA_Players->player[this->getID()].force].name;
  }
  //----------------------------------------------------------------------------------------------------------
  void Player::onGameEnd()
  {
    this->units.clear();
    this->leftTheGame=false;
  }
  //---------------------------------------------- LEFT GAME -------------------------------------------------
  bool Player::leftGame() const
  {
    return this->leftTheGame;
  }
  //----------------------------------------------------------------------------------------------------------
};
