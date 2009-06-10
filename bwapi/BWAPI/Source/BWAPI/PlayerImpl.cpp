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
    :id(id)
    ,unitCacheFrame(-1)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  PlayerImpl::~PlayerImpl()
  {
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  std::string PlayerImpl::getName() const
  {
    return std::string(BW::BWDATA_Players->player[this->getID()].name);
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
  //------------------------------------------------- GET ID -------------------------------------------------
  u8 PlayerImpl::getID() const
  {
    return this->id;
  }
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  BW::Unit** PlayerImpl::selectedUnit()
  {
    return (BW::Unit**)(BW::BWDATA_PlayerSelection + this->getID()*48);
  }
  #pragma warning(pop)
  //------------------------------------------- GET MINERALS LOCAL -------------------------------------------
  int PlayerImpl::minerals() const
  {
    return this->mineralsLocal;
  }
  //--------------------------------------------- GET GAS LOCAL ----------------------------------------------
  int PlayerImpl::gas() const
  {
    return this->gasLocal;
  }
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
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  s32 PlayerImpl::supplyTotal() const
  {
    s32 ret = this->suppliesAvailableLocal[static_cast<BW::Race::Enum>(getRace().getID())];
    return ret < getSuppliesMaxSync(static_cast<BW::Race::Enum>(getRace().getID())) ? ret : getSuppliesMaxSync(static_cast<BW::Race::Enum>(getRace().getID()));
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  s32 PlayerImpl::supplyUsed() const
  {
    return this->suppliesUsedLocal[getRace().getID()];
  }
  //--------------------------------------- USE SUPPLIES PROTOSS LOCAL ---------------------------------------
  void PlayerImpl::useSupplies(u8 supplies, BW::Race::Enum race)
  {
    this->suppliesUsedLocal[race] += supplies;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BWAPI::Race PlayerImpl::getRace() const
  {
    return BWAPI::Race((int)(BW::BWDATA_Players->player[this->getID()].race));
  }
  //------------------------------------------------ GET ALLIANCE --------------------------------------------
  u8 PlayerImpl::getAlliance(u8 opposingID)
  {
    return BW::BWDATA_Alliance->alliance[this->getID()].player[opposingID];
  }
  //------------------------------------------------ GET OWNER -----------------------------------------------
  BW::PlayerType::Enum PlayerImpl::playerType() const
  {
    return BW::BWDATA_Players->player[this->getID()].type;
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  u8 PlayerImpl::getForce()
  {
    return BW::BWDATA_Players->player[this->getID()].force;
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  Force* PlayerImpl::getForce() const
  {
    return NULL;//TODO: create Force class
  }
  //--------------------------------------------- GET ALL UNITS ----------------------------------------------
  s32 PlayerImpl::getAllUnits(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->all, unit) + this->toMake[unit.getID()];
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 PlayerImpl::getCompletedUnits(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->completed, unit);
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 PlayerImpl::getCompletedUnits(BW::UnitType unit, BWAPI::Race race)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->completed, unit, static_cast<BW::Race::Enum>(race.getID()));
  }  
  //------------------------------------------ GET INCOMPLETE UNITS ------------------------------------------
  s32 PlayerImpl::getIncompleteUnits(BW::UnitType unit)
  {
    return this->getAllUnits(unit) - this->getCompletedUnits(unit) + toMake[unit.getID()];
  }
  //----------------------------------------------- GET DEATHS -----------------------------------------------
  s32 PlayerImpl::getDeaths(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->dead, unit);
  }
  //----------------------------------------------- GET KILLS ------------------------------------------------
  s32 PlayerImpl::getKills(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->killed, unit);
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  s32 PlayerImpl::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit)
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Infantry)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canAttack() &&
			BW::UnitType((BW::UnitID::Enum)i).isOrganic() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Mech)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isMechanical() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_Main)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Hatchery ||
            unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_MainLair)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += counts.unit[i].player[this->getID()];
    return temp;
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  s32 PlayerImpl::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit, BW::Race::Enum race)
  {
    if(unit.getID() < BW::UnitID::None)
      return counts.unit[unit.getID()].player[this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
          temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

	    if (unit == BW::UnitID::Infantry)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canAttack() &&
			BW::UnitType((BW::UnitID::Enum)i).isOrganic() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Mech)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
		  if (BW::UnitType((BW::UnitID::Enum)i).isMechanical() &&
			!BW::UnitType((BW::UnitID::Enum)i).isWorker())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_Main)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Hatchery ||
            unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Zerg_MainLair)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (unit == BW::UnitID::Zerg_Lair ||
            unit == BW::UnitID::Zerg_Hive)
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          if (BW::UnitType((BW::UnitID::Enum)i).getRace() == race)
            temp += counts.unit[i].player[this->getID()];
    return temp;
  }  
  //------------------------------------------- PLAN TO MAKE -------------------------------------------------
  void PlayerImpl::planToMake(BW::UnitType unit)
  {
    this->toMake[unit.getID()]++;
  }
  //------------------------------------------- GET FORCE NAME -----------------------------------------------
  char* PlayerImpl::getForceName() const
  {
    return BW::ForceNames[BW::BWDATA_Players->player[this->getID()].force].name;
  }
  //------------------------------------------ RESEARCH IN PROGRESS ------------------------------------------
  bool PlayerImpl::researching(BWAPI::TechType tech) const
  {
    Util::BitMask<u64>* techs = (Util::BitMask<u64>*) (BW::BWDATA_ResearchProgress + this->getID()*6);
    return techs->getBit(1 << tech.getID());
  }
  //-------------------------------------------- TECH RESEARCHED ---------------------------------------------
  bool PlayerImpl::researched(BWAPI::TechType tech) const
  {
   if (tech.getID() < 0x18)
     return *((u8*)(BW::BWDATA_TechResearchSC + this->getID()*0x18 + tech.getID())) == 1;
   else
     return *((u8*)(BW::BWDATA_TechResearchBW + this->getID()*0x14 + tech.getID() - 0x18)) == 1;
  }
  //--------------------------------------------- UPGRADE LEVEL ----------------------------------------------
  int PlayerImpl::upgradeLevel(UpgradeType upgrade) const
  {
    if (upgrade.getID() < 46)
     return (int)(*((u8*)(BW::BWDATA_UpgradeLevelSC + this->getID()*46 + upgrade.getID())));
   else
     return (int)(*((u8*)(BW::BWDATA_UpgradeLevelBW + this->getID()*15 + upgrade.getID() - 46)));
  }
  //------------------------------------------ UPGRADE IN PROGRESS -------------------------------------------
  bool PlayerImpl::upgrading(UpgradeType upgrade) const
  {
    return BW::BWDATA_UpgradeProgress->player[this->getID()].getBit(1 << upgrade.getID());
  }
  //----------------------------------------------- GET UNITS ------------------------------------------------
  std::set<Unit*> PlayerImpl::getUnits()
  {
    if (this->unitCacheFrame!=BWAPI::BroodwarImpl.getFrameCount())
    {
      this->units.clear();
      for(std::set<UnitImpl*>::iterator u=BWAPI::BroodwarImpl.units.begin();u!=BWAPI::BroodwarImpl.units.end();u++)
      {
        if ((*u)->getOwner()==this)
        {
          this->units.insert((Unit*)(*u));
        }
      }
      this->unitCacheFrame=BWAPI::BroodwarImpl.getFrameCount();
    }
    return this->units;
  }
  //----------------------------------------------------------------------------------------------------------
};
