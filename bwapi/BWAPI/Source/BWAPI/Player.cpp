#include "Player.h"

#include <BW/Offsets.h>
#include <BW/UnitID.h>

namespace BWAPI
{
  //------------------------------ CONSTRUCTOR --------------------------------
  Player::Player(u8 id)
    :id(id)
  {
  }
  //------------------------------- DESTRUCTOR --------------------------------
  Player::~Player()
  {
  }
  //-------------------------------- GET NAME ---------------------------------
  char* Player::getName() const
  {
    return BW::BWXFN_Players->player[this->getID()].name;
  }
  //------------------------------- GET MINERALS ------------------------------
  s32 Player::getMinerals() const
  {
    return BW::BWXFN_PlayerResources->minerals.player[this->getID()];
  }
  //--------------------------------- GET GAS ---------------------------------
  s32 Player::getGas() const
  {
    return BW::BWXFN_PlayerResources->gas.player[this->getID()];
  }
  //---------------------------------- GET ID ---------------------------------
  u8 Player::getID() const
  {
    return this->id;
  }
  //------------------------------- SELECTED UNIT -----------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  BW::Unit** Player::selectedUnit()
  {
    return (BW::Unit**)(0x006284D0 + this->getID()*48);
  }
  #pragma warning(pop)
  //----------------------------- GET MINERALS LOCAL --------------------------
  s32 Player::getMineralsLocal() const
  {
    return this->mineralsLocal;
  }
  //------------------------------- GET GAS LOCAL -----------------------------
  s32 Player::getGasLocal() const
  {
    return this->gasLocal;
  }
  //---------------------------------- UPDATE ---------------------------------
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
  //-------------------------------- SPEND LOCAL ------------------------------
  void  Player::spendLocal(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //---------------------------- GET SUPPLY AVAILABLE ------------------------
  s32 Player::getSuppliesAvailable(BW::Race::Enum race)
  {
    s32 ret = BW::BWXFN_Supplies->race[race].available.player[this->getID()];
    return ret < 400 ? ret : 400;
  }
  //------------------------- GET SUPPLY USED ---------------------------------
  s32 Player::getSuppliesUsed(BW::Race::Enum race)
  {
    return BW::BWXFN_Supplies->race[race].used.player[this->getID()];
  }
  //------------------------- GET SUPPLY AVAILABLE LOCAL ----------------------
  s32 Player::getSuppliesAvailableLocal(BW::Race::Enum race)
  {
    s32 ret = this->suppliesAvailableLocal[race];
    return ret < 400 ? ret : 400;
  }
  //------------------------- GET SUPPLY USED LOCAL ---------------------------
  s32 Player::getSuppliesUsedLocal(BW::Race::Enum race)
  {
    return this->suppliesUsedLocal[race];
  }
  //---------------------------------------------------------------------------
  s32 Player::getSuppliesFreeLocal(BW::Race::Enum race)
  {
    return this->getSuppliesAvailableLocal(race) - this->getSuppliesUsedLocal(race);
  }
  //------------------------------ USE SUPPLIES PROTOSS LOCAL -----------------
  void Player::useSuppliesLocal(u8 supplies, BW::Race::Enum race)
  {
    this->suppliesUsedLocal[race] += supplies;
  }
  //--------------------------------- CAN BUILD -------------------------------
  bool Player::canBuild(BW::UnitType unit)
  {
    switch (unit.getID())
    {
     case BW::UnitID::Terran_Medic   : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0;
     case BW::UnitID::Terran_Firebat : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0;
     case BW::UnitID::Terran_Ghost   : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0 &&
                                              this->unitTypeCount[BW::UnitID::Terran_CovertOps] != 0;
     case BW::UnitID::Terran_ComsatStation   : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0;
     default : return true;
    }
  }
  //--------------------------------- CAN AFFORD ------------------------------
  bool Player::canAfford(BW::UnitType unit, u16 mineralsToKeep)
  {
    return this->getSuppliesFreeLocal(unit.getRace()) >= unit.getSupplies() &&
           this->getMineralsLocal() - mineralsToKeep >= unit.getMineralPrice() &&
           this->getGasLocal() >= unit.getGasPrice();
  }
  //--------------------------------- GET RACE --------------------------------
  BW::Race::Enum Player::getRace()
  {
    return BW::BWXFN_Players->player[this->getID()].race;
  }
  //---------------------------------------------------------------------------
};
