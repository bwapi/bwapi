#include "Player.h"

#include "../BW/Offsets.h"
#include "../BW/UnitID.h"

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
    return BW::BWXFN_PlayerNames->player[this->getID()].name;
  }
  //-------------------------------- SET NAME ---------------------------------
  void Player::setName(const char* const name)
  {
    strcpy(BW::BWXFN_PlayerNames->player[this->getID()].name, name);
  }
  //------------------------------- GET MINERALS ------------------------------
  s32 Player::getMinerals() const
  {
    return BW::BWXFN_PlayerMinerals->player[this->getID()];
  }
  //--------------------------------- GET GAS ---------------------------------
  s32 Player::getGas() const
  {
    return BW::BWXFN_PlayerGas->player[this->getID()];
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
    this->suppliesAvailableTerranLocal = this->getSuppliesAvailable(BW::Race::Terran);
    this->suppliesUsedTerranLocal = this->getSuppliesUsed(BW::Race::Terran);
    this->suppliesAvailableProtossLocal = this->getSuppliesAvailable(BW::Race::Protoss);
    this->suppliesUsedProtossLocal = this->getSuppliesUsed(BW::Race::Protoss);
    this->suppliesAvailableZergLocal = this->getSuppliesAvailable(BW::Race::Zerg);
    this->suppliesUsedZergLocal = this->getSuppliesUsed(BW::Race::Zerg);
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
    s32 ret;
    switch (race)
    {
    case BW::Race::Terran: ret = BW::BWXFN_SuppliesAvailableTerran->player[this->getID()].suppliesAvailableTerran; 
      break;
    case BW::Race::Zerg: ret = BW::BWXFN_SuppliesAvailableZerg->player[this->getID()].suppliesAvailableZerg;
      break;
    case BW::Race::Protoss: ret = BW::BWXFN_SuppliesAvailableProtoss->player[this->getID()].suppliesAvailableProtoss;
      break;
    default: return 0;
    }
    return ret < 400 ? ret : 400;
  }
  //------------------------- GET SUPPLY USED ---------------------------------
  s32 Player::getSuppliesUsed(BW::Race::Enum race)
  {
    switch (race)
    {
      case BW::Race::Terran: return BW::BWXFN_SuppliesAvailableTerran->player[this->getID()].suppliesAvailableTerran;
      case BW::Race::Zerg: return BW::BWXFN_SuppliesUsedZerg->player[this->getID()].suppliesUsedZerg;
      case BW::Race::Protoss: return BW::BWXFN_SuppliesUsedProtoss->player[this->getID()].suppliesUsedProtoss;
    }
    return 0;
  }
  //------------------------- GET SUPPLY AVAILABLE LOCAL ----------------------
  s32 Player::getSuppliesAvailableLocal(BW::Race::Enum race)
  {
    s32 ret;
    switch (race)
    {
    case BW::Race::Terran: ret = this->suppliesAvailableTerranLocal;
      break;
    case BW::Race::Zerg: ret = this->suppliesAvailableZergLocal;
      break;
    case BW::Race::Protoss: ret = this->suppliesAvailableProtossLocal;
      break;
    default: return 0;
    }
    return ret < 400 ? ret : 400;
  }
  //------------------------- GET SUPPLY USED LOCAL ---------------------------
  s32 Player::getSuppliesUsedLocal(BW::Race::Enum race)
  {
    switch (race)
    {
      case BW::Race::Terran: return this->suppliesUsedTerranLocal;
      case BW::Race::Zerg: return this->suppliesUsedZergLocal;
      case BW::Race::Protoss: return this->suppliesUsedProtossLocal;
    }
    return 0;
  }
  //---------------------------------------------------------------------------
  s32 Player::freeSuppliesLocal(BW::Race::Enum race)
  {
    return this->getSuppliesAvailableLocal(race) - this->getSuppliesUsedLocal(race);
  }
  //---------------------------------------------------------------------------
  s32 Player::usedSuppliesLocal(BW::Race::Enum race)
  {
    return this->getSuppliesUsedLocal(race);
  }
  //------------------------------ USE SUPPLIES PROTOSS LOCAL -----------------
  void Player::useSuppliesProtossLocal(u8 supplies)
  {
    this->suppliesUsedProtossLocal += supplies;
  }
  //------------------------------ USE SUPPLIES TERRAN LOCAL ------------------
  void Player::useSuppliesTerranLocal(u8 supplies)
  {
    this->suppliesUsedTerranLocal += supplies;
  }
  //------------------------------ USE SUPPLIES ZERG LOCAL --------------------
  void Player::useSuppliesZergLocal(u8 supplies)
  {
    this->suppliesUsedZergLocal += supplies;
  }
  //--------------------------------- CAN BUILD -------------------------------
  bool Player::canBuild(BW::UnitType unit)
  {
    switch (unit.getID())
    {
     case BW::UnitID::Terran_Medic : return this->unitTypeCount[BW::UnitID::Terran_Academy] != 0;
     default : return true;
    }
  }
  //--------------------------------- CAN AFFORD ------------------------------
  bool Player::canAfford(BW::UnitType unit, u16 mineralsToKeep)
  {
    return ( 
             ( 
               unit.isTerran() &&
               this->freeSuppliesLocal(BW::Race::Terran) >= unit.getSupplies()
             ) ||
             (
               unit.isProtoss() &&
               this->freeSuppliesLocal(BW::Race::Protoss) >= unit.getSupplies()
             ) ||
             (
               unit.isZerg() &&
               this->freeSuppliesLocal(BW::Race::Zerg) >= unit.getSupplies()
             ) 
           ) &&
           this->getMineralsLocal() - mineralsToKeep >= unit.getMineralPrice() &&
           this->getGasLocal() >= unit.getGasPrice();
  }
};
