#include "Player.h"

#include <Util/Bitmask.h>

#include <BW/Offsets.h>
#include <BW/UnitID.h>

namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Player::Player(u8 id)
    :id(id)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Player::~Player()
  {
  }
  //------------------------------------------------ GET NAME ------------------------------------------------
  char* Player::getName() const
  {
    return BW::BWDATA_Players->player[this->getID()].name;
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
  //------------------------------------------------- GET ID -------------------------------------------------
  u8 Player::getID() const
  {
    return this->id;
  }
  //--------------------------------------------- SELECTED UNIT ----------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  BW::Unit** Player::selectedUnit()
  {
    return (BW::Unit**)(BW::BWDATA_PlayerSelection + this->getID()*48);
  }
  #pragma warning(pop)
  //------------------------------------------- GET MINERALS LOCAL -------------------------------------------
  s32 Player::getMinerals() const
  {
    return this->mineralsLocal;
  }
  //--------------------------------------------- GET GAS LOCAL ----------------------------------------------
  s32 Player::getGas() const
  {
    return this->gasLocal;
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void Player::update()
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
  void  Player::spend(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //------------------------------------------ GET SUPPLY AVAILABLE ------------------------------------------
  s32 Player::getSuppliesAvailableSync(BW::Race::Enum race)
  {
    s32 ret = BW::BWDATA_Supplies->race[race].available.player[this->getID()];
    return ret < getSuppliesMaxSync(race) ? ret : getSuppliesMaxSync(race);
  }
  //-------------------------------------------- GET SUPPLY USED ---------------------------------------------
  s32 Player::getSuppliesUsedSync(BW::Race::Enum race)
  {
    return BW::BWDATA_Supplies->race[race].used.player[this->getID()];
  }
  //--------------------------------------------- GET SUPPLY MAX ---------------------------------------------
  s32 Player::getSuppliesMaxSync(BW::Race::Enum race)
  {
    return BW::BWDATA_Supplies->race[race].max.player[this->getID()];
  }
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  s32 Player::getSuppliesAvailable(BW::Race::Enum race)
  {
    s32 ret = this->suppliesAvailableLocal[race];
    return ret < getSuppliesMaxSync(race) ? ret : getSuppliesMaxSync(race);
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  s32 Player::getSuppliesUsed(BW::Race::Enum race)
  {
    return this->suppliesUsedLocal[race];
  }
  //------------------------------------------ SUPPLIES FREE LOCAL -------------------------------------------
  s32 Player::getSuppliesFree(BW::Race::Enum race)
  {
    return this->getSuppliesAvailable(race) - this->getSuppliesUsed(race);
  }
  //--------------------------------------- USE SUPPLIES PROTOSS LOCAL ---------------------------------------
  void Player::useSupplies(u8 supplies, BW::Race::Enum race)
  {
    this->suppliesUsedLocal[race] += supplies;
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BW::Race::Enum Player::getRace()
  {
    return BW::BWDATA_Players->player[this->getID()].race;
  }
  //------------------------------------------------ GET ALLIANCE --------------------------------------------
  u8 Player::getAlliance(u8 opposingID)
  {
    return BW::BWDATA_Alliance->alliance[this->getID()].player[opposingID];
  }
  //------------------------------------------------ GET OWNER -----------------------------------------------
  BW::PlayerType::Enum Player::getOwner()
  {
    return BW::BWDATA_Players->player[this->getID()].type;
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  u8 Player::getForce()
  {
    return BW::BWDATA_Players->player[this->getID()].force;
  }
  //--------------------------------------------- GET ALL UNITS ----------------------------------------------
  s32 Player::getAllUnits(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->all, unit) + this->toMake[unit.getID()];
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 Player::getCompletedUnits(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->completed, unit);
  }
  //------------------------------------------ GET COMPLETED UNITS -------------------------------------------
  s32 Player::getCompletedUnits(BW::UnitType unit, BW::Race::Enum race)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->completed, unit, race);
  }  
  //------------------------------------------ GET INCOMPLETE UNITS ------------------------------------------
  s32 Player::getIncompleteUnits(BW::UnitType unit)
  {
    return this->getAllUnits(unit) - this->getCompletedUnits(unit) + toMake[unit.getID()];
  }
  //----------------------------------------------- GET DEATHS -----------------------------------------------
  s32 Player::getDeaths(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->dead, unit);
  }
  //----------------------------------------------- GET KILLS ------------------------------------------------
  s32 Player::getKills(BW::UnitType unit)
  {
    return this->evaluateCounts(BW::BWDATA_Counts->killed, unit);
  }
  //-------------------------------------------- EVALUATE COUNTS ---------------------------------------------
  s32 Player::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit)
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
  s32 Player::evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit, BW::Race::Enum race)
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
  void Player::planToMake(BW::UnitType unit)
  {
    this->toMake[unit.getID()]++;
  }
  //------------------------------------------- GET FORCE NAME -----------------------------------------------
  char* Player::getForceName() const
  {
    return BW::ForceNames[BW::BWDATA_Players->player[this->getID()].force].name;
  }
  //------------------------------------------ RESEARCH IN PROGRESS ------------------------------------------
  bool Player::researchInProgress(BW::TechType tech) const
  {
    Util::BitMask<u64>* techs = (Util::BitMask<u64>*) (BW::BWDATA_ResearchProgress + this->getID()*6);
    return techs->getBit(1 << tech.getID());
  }
  //-------------------------------------------- TECH RESEARCHED ---------------------------------------------
  bool Player::techResearched(BW::TechType tech) const
  {
   if (tech.getID() < 0x18)
     return *((u8*)(BW::BWDATA_TechResearchSC + this->getID()*0x18 + tech.getID())) == 1;
   else
     return *((u8*)(BW::BWDATA_TechResearchBW + this->getID()*0x14 + tech.getID() - 0x18)) == 1;
  }
  //--------------------------------------------- UPGRADE LEVEL ----------------------------------------------
  u8 Player::upgradeLevel(BW::UpgradeType upgrade) const
  {
    if (upgrade.getID() < 46)
     return *((u8*)(BW::BWDATA_UpgradeLevelSC + this->getID()*46 + upgrade.getID()));
   else
     return *((u8*)(BW::BWDATA_UpgradeLevelBW + this->getID()*15 + upgrade.getID() - 46));
  }
  //------------------------------------------ UPGRADE IN PROGRESS -------------------------------------------
  bool Player::upgradeInProgress(BW::UpgradeType upgrade) const
  {
    return BW::BWDATA_UpgradeProgress->player[this->getID()].getBit(1 << upgrade.getID());
  }  
  //----------------------------------------------------------------------------------------------------------
};
