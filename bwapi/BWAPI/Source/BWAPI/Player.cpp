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
    return BW::BWXFN_Players->player[this->getID()].name;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  s32 Player::getMinerals() const
  {
    return BW::BWXFN_PlayerResources->minerals.player[this->getID()];
  }
  //------------------------------------------------ GET GAS -------------------------------------------------
  s32 Player::getGas() const
  {
    return BW::BWXFN_PlayerResources->gas.player[this->getID()];
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
    return (BW::Unit**)(0x006284D0 + this->getID()*48);
  }
  #pragma warning(pop)
  //------------------------------------------- GET MINERALS LOCAL -------------------------------------------
  s32 Player::getMineralsLocal() const
  {
    return this->mineralsLocal;
  }
  //--------------------------------------------- GET GAS LOCAL ----------------------------------------------
  s32 Player::getGasLocal() const
  {
    return this->gasLocal;
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void Player::update()
  {
    this->mineralsLocal = this->getMinerals();
    this->gasLocal = this->getGas();
    for (int i = 0; i < BW::RACE_COUNT; i++)
    {
      this->suppliesAvailableLocal[i] = this->getSuppliesAvailable((BW::Race::Enum)i);
      this->suppliesUsedLocal[i] = this->getSuppliesUsed((BW::Race::Enum)i);
    }
  }
  //---------------------------------------------- SPEND LOCAL -----------------------------------------------
  void  Player::spendLocal(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //------------------------------------------ GET SUPPLY AVAILABLE ------------------------------------------
  s32 Player::getSuppliesAvailable(BW::Race::Enum race)
  {
    s32 ret = BW::BWXFN_Supplies->race[race].available.player[this->getID()];
    return ret < getSuppliesMax(race) ? ret : getSuppliesMax(race);
  }
  //-------------------------------------------- GET SUPPLY USED ---------------------------------------------
  s32 Player::getSuppliesUsed(BW::Race::Enum race)
  {
    return BW::BWXFN_Supplies->race[race].used.player[this->getID()];
  }
  //-------------------------------------------- GET SUPPLY MAX ----------------------------------------------
  s32 Player::getSuppliesMax(BW::Race::Enum race)
  {
    return BW::BWXFN_Supplies->race[race].max.player[this->getID()];
  }
  //--------------------------------------- GET SUPPLY AVAILABLE LOCAL ---------------------------------------
  s32 Player::getSuppliesAvailableLocal(BW::Race::Enum race)
  {
    s32 ret = this->suppliesAvailableLocal[race];
    return ret < getSuppliesMax(race) ? ret : getSuppliesMax(race);
  }
  //----------------------------------------- GET SUPPLY USED LOCAL ------------------------------------------
  s32 Player::getSuppliesUsedLocal(BW::Race::Enum race)
  {
    return this->suppliesUsedLocal[race];
  }
  //------------------------------------------ SUPPLIES FREE LOCAL -------------------------------------------
  s32 Player::getSuppliesFreeLocal(BW::Race::Enum race)
  {
    return this->getSuppliesAvailableLocal(race) - this->getSuppliesUsedLocal(race);
  }
  //--------------------------------------- USE SUPPLIES PROTOSS LOCAL ---------------------------------------
  void Player::useSuppliesLocal(u8 supplies, BW::Race::Enum race)
  {
    this->suppliesUsedLocal[race] += supplies;
  }
  //----------------------------------------------- CAN BUILD ------------------------------------------------
  bool Player::canBuild(BW::UnitType unit)
  {
    switch (unit.getID())
    {
     case BW::UnitID::Terran_Medic           : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0;
     case BW::UnitID::Terran_Firebat         : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0;
     case BW::UnitID::Terran_Ghost           : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0 &&
                                                      this->unitTypeCount[BW::UnitID::Terran_CovertOps] != 0;
     case BW::UnitID::Terran_ComsatStation   : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0;
     case BW::UnitID::Terran_Factory         : return this->unitTypeCount[BW::UnitID::Terran_Barracks] != 0;
     case BW::UnitID::Terran_Starport        : return this->unitTypeCount[BW::UnitID::Terran_Factory] != 0;
     case BW::UnitID::Terran_ControlTower    : return this->unitTypeCount[BW::UnitID::Terran_Starport] != 0;
     case BW::UnitID::Terran_ScienceFacility : return this->unitTypeCount[BW::UnitID::Terran_Starport] != 0;
     case BW::UnitID::Terran_CovertOps       : return this->unitTypeCount[BW::UnitID::Terran_ScienceFacility] != 0;
     default                                 : return true;
    }
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::UnitType unit, const BWAPI::ReservedResources& reserved)
  {
    return ((int)this->getSuppliesFreeLocal(unit.getRace())) - reserved.supply   >= unit.getSupplies() &&
           ((int)this->getMineralsLocal())                   - reserved.minerals >= unit.getMineralPrice() &&
           ((int)this->getGasLocal())                        - reserved.gas      >= unit.getGasPrice();
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::TechType tech, const BWAPI::ReservedResources& reserved)
  {
    return ((int)this->getMineralsLocal()) - reserved.minerals >= tech.getMineralPrice() &&
           ((int)this->getGasLocal())      - reserved.gas      >= tech.getGasPrice();
  }
  //----------------------------------------------- CAN AFFORD -----------------------------------------------
  bool Player::canAfford(BW::UpgradeType upgrade, u8 level, const BWAPI::ReservedResources& reserved)
  {
    return ((int)this->getMineralsLocal()) - reserved.minerals >= upgrade.mineralCostBase() + upgrade.mineralCostFactor()*(level-1) &&
           ((int)this->getGasLocal())      - reserved.gas      >= upgrade.gasCostBase()     + upgrade.gasCostFactor()    *(level-1);
  }
  //------------------------------------------------ GET RACE ------------------------------------------------
  BW::Race::Enum Player::getRace()
  {
    return BW::BWXFN_Players->player[this->getID()].race;
  }
  //------------------------------------------------ GET FORCE ------------------------------------------------
  u8 Player::getForce()
  {
    return BW::BWXFN_Players->player[this->getID()].force;
  }
  //------------------------------------------------ GET ALL UNITS -------------------------------------------
  s32 Player::getAllUnits(BW::UnitType unit)
  {
    if(unit.getID() < BW::UnitID::None)
      return BW::CountAllUnits[BW::PLAYER_COUNT * unit.getID() + this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()];
    return temp;
  }
  //------------------------------------------------ GET DEATHS ----------------------------------------------
  s32 Player::getDeaths(BW::UnitType unit)
  {
    if(unit.getID() < BW::UnitID::None)
      return BW::CountDeadUnits[BW::PLAYER_COUNT * unit.getID() + this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        temp += BW::CountDeadUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountDeadUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          temp += BW::CountDeadUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountDeadUnits[BW::PLAYER_COUNT * i + this->getID()];
    return temp;
  }
  //------------------------------------------------ GET KILLS -----------------------------------------------
  s32 Player::getKills(BW::UnitType unit)
  {
    if(unit.getID() < BW::UnitID::None)
      return BW::CountKilledUnits[BW::PLAYER_COUNT * unit.getID() + this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        temp += BW::CountKilledUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountKilledUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          temp += BW::CountKilledUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountKilledUnits[BW::PLAYER_COUNT * i + this->getID()];
    return temp;
  }
  //------------------------------------------------ GET COMPLETED UNITS -------------------------------------
  s32 Player::getCompletedUnits(BW::UnitType unit)
  {
    if(unit.getID() < BW::UnitID::None)
      return BW::CountCompletedUnits[BW::PLAYER_COUNT * unit.getID() + this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        temp += BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          temp += BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];
    return temp;
  }
  //------------------------------------------------ GET INCOMPLETE UNITS ------------------------------------
  s32 Player::getIncompleteUnits(BW::UnitType unit)
  {
     if(unit.getID() < BW::UnitID::None)
      return BW::CountAllUnits[BW::PLAYER_COUNT * unit.getID() + this->getID()] - BW::CountCompletedUnits[BW::PLAYER_COUNT * unit.getID() + this->getID()];

    s32 temp = 0;
    if (unit == BW::UnitID::All)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()] - BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Buildings)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()] - BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Factories)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (BW::UnitType((BW::UnitID::Enum)i).canProduce())
          temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()] - BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];

    if (unit == BW::UnitID::Men)
      for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        if (!BW::UnitType((BW::UnitID::Enum)i).isBuilding())
          temp += BW::CountAllUnits[BW::PLAYER_COUNT * i + this->getID()] - BW::CountCompletedUnits[BW::PLAYER_COUNT * i + this->getID()];
    return temp;
  }
  //------------------------------------------------ GET FORCE NAME ------------------------------------------
  char* Player::getForceName() const
  {
    return BW::ForceNames[BW::BWXFN_Players->player[this->getID()].force].name;
  }
  //------------------------------------------ RESEARCH IN PROGRESS ------------------------------------------
  bool Player::researchInProgress(BW::TechType tech) const
  {
    Util::BitMask<u64>* techs = (Util::BitMask<u64>*) (0x0058F218 + this->getID()*6);
    return techs->getBit(1 << tech.getID());
  }
  //-------------------------------------------- TECH RESEARCHED ---------------------------------------------
  bool Player::techResearched(BW::TechType tech) const
  {
   if (tech.getID() < 0x18)
     return *((u8*)(0x0058CF2C + this->getID()*0x18 + tech.getID())) == 1;
   else
     return *((u8*)(0x0058F128 + this->getID()*0x14 + tech.getID() - 0x18)) == 1;
  }
  //--------------------------------------------- UPGRADE LEVEL ----------------------------------------------
  u8 Player::upgradeLevel(BW::UpgradeType upgrade) const
  {
    if (upgrade.getID() < 46)
     return *((u8*)(0x0058D298 + this->getID()*46 + upgrade.getID()));
   else
     return *((u8*)(0x0058F314 + this->getID()*15 + upgrade.getID() - 46));
  }
  //------------------------------------------ UPGRADE IN PROGRESS -------------------------------------------
  bool Player::upgradeInProgress(BW::UpgradeType upgrade) const
  {
    return BW::BWXFN_UpgradeProgress->player[this->getID()].getBit(1 << upgrade.getID());
  }  
  //----------------------------------------------------------------------------------------------------------
};
