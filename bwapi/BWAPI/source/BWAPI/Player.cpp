#include "Player.h"
#include "../BW/Offsets.h"

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
    return BW::PlayerNames->player[this->getID()].name;
  }
  //-------------------------------- SET NAME ---------------------------------
  void Player::setName(const char* const name)
  {
    strcpy(BW::PlayerNames->player[this->getID()].name, name);
  }
  //------------------------------- GET MINERALS ------------------------------
  s32 Player::getMinerals() const
  {
    return BW::PlayerMinerals->player[this->getID()];
  }
  //--------------------------------- GET GAS ---------------------------------
  s32 Player::getGas() const
  {
    return BW::PlayerGas->player[this->getID()];
  }
  //---------------------------------- GET ID ---------------------------------
  u8 Player::getID() const
  {
    return this->id;
  }
  //------------------------------- SELECTED UNIT -----------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  BW::UnitData** Player::selectedUnit()
  {
    return (BW::UnitData**)(0x006284D0 + this->getID()*48);
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
    this->suppliesAvailableTerranLocal = this->getSuppliesAvailableTerran();
    this->suppliesUsedTerranLocal = this->getSuppliesUsedTerran();
  }
  //-------------------------------- SPEND LOCAL ------------------------------
  void  Player::spendLocal(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
   //---------------------------- GET SUPPLY AVAILABLE PROTOSS ----------------
  s32 Player::getSuppliesAvailableProtoss()
  {
     return BW::SuppliesAvaialbeProtoss->player[this->getID()].suppliesAvailableProtoss;
  }
  //------------------------- GET SUPPLY AVAILABLE PROTOSS LOCAL --------------
  s32 Player::getSuppliesAvailableProtossLocal()
  {
    return this->suppliesAvailableProtossLocal;
  }
  //------------------------------ GET SUPPLY USED PROTOSS --------------------
  s32 Player::getSuppliesUsedProtoss()
  {
   return BW::SuppliesUsedProtoss->player[this->getID()].suppliesUsedProtoss;
  }
  //------------------------- GET SUPPLY AVAILABLE PROTOSS LOCAL --------------
  s32 Player::getSuppliesUsedProtossLocal()
  {
    return this->suppliesUsedProtossLocal;
  }
  //------------------------------ USE SUPPLIES PROTOSS LOCAL -----------------
  void Player::useSuppliesProtossLocal(u8 supplies)
  {
    this->suppliesUsedProtossLocal += supplies;
  }
  //------------------------------- FREE SUPPLIES PROTOSS LOCAL ---------------
  s32 Player::freeSuppliesProtossLocal()
  {
    return this->getSuppliesAvailableProtossLocal() - this->getSuppliesUsedProtossLocal();
  }
  //---------------------------- GET SUPPLY AVAILABLE TERRAN ------------------
  s32 Player::getSuppliesAvailableTerran()
  {
     return BW::SuppliesAvaialbeTerran->player[this->getID()].suppliesAvailableTerran;
  }
  //------------------------- GET SUPPLY AVAILABLE TERRAN LOCAL ---------------
  s32 Player::getSuppliesAvailableTerranLocal()
  {
    return this->suppliesAvailableTerranLocal;
  }
  //------------------------------ GET SUPPLY USED TERRAN ---------------------
  s32 Player::getSuppliesUsedTerran()
  {
   return BW::SuppliesUsedTerran->player[this->getID()].suppliesUsedTerran;
  }
  //------------------------- GET SUPPLY AVAILABLE TERRAN LOCAL ---------------
  s32 Player::getSuppliesUsedTerranLocal()
  {
    return this->suppliesUsedTerranLocal;
  }
  //------------------------------ USE SUPPLIES TERRAN LOCAL ------------------
  void Player::useSuppliesTerranLocal(u8 supplies)
  {
    this->suppliesUsedTerranLocal += supplies;
  }
  //------------------------------- FREE SUPPLIES TERRAN LOCAL ----------------
  s32 Player::freeSuppliesTerranLocal()
  {
    return this->getSuppliesAvailableTerranLocal() - this->getSuppliesUsedTerranLocal();
  }
  //---------------------------- GET SUPPLY AVAILABLE ZERG --------------------
  s32 Player::getSuppliesAvailableZerg()
  {
     return BW::SuppliesAvaialbeZerg->player[this->getID()].suppliesAvailableZerg;
  }
  //------------------------- GET SUPPLY AVAILABLE ZERG LOCAL -----------------
  s32 Player::getSuppliesAvailableZergLocal()
  {
    return this->suppliesAvailableZergLocal;
  }
  //------------------------------ GET SUPPLY USED ZERG -----------------------
  s32 Player::getSuppliesUsedZerg()
  {
   return BW::SuppliesUsedZerg->player[this->getID()].suppliesUsedZerg;
  }
  //------------------------- GET SUPPLY AVAILABLE ZERG LOCAL -----------------
  s32 Player::getSuppliesUsedZergLocal()
  {
    return this->suppliesUsedZergLocal;
  }
  //------------------------------ USE SUPPLIES ZERG LOCAL --------------------
  void Player::useSuppliesZergLocal(u8 supplies)
  {
    this->suppliesUsedZergLocal += supplies;
  }
  //------------------------------- FREE SUPPLIES ZERG LOCAL ------------------
  s32 Player::freeSuppliesZergLocal()
  {
    return this->getSuppliesAvailableZergLocal() - this->getSuppliesUsedZergLocal();
  }
  //---------------------------------------------------------------------------
};
