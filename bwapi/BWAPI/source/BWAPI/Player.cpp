#include "Player.h"
#include "../BW/Offsets.h"

namespace BWAPI
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Player::Player(u8 id)
    :id(id)
  {
  }
  //------------------------------- DESTRUCTOR ----------------------------------
  Player::~Player()
  {
  }
  //-------------------------------- GET NAME -----------------------------------
  char* Player::getName() const
  {
    return BW::PlayerNames->player[this->getID()].name;
  }
  //-------------------------------- SET NAME -----------------------------------
  void Player::setName(const char* const name)
  {
    strcpy(BW::PlayerNames->player[this->getID()].name, name);
  }
  //------------------------------- GET MINERALS --------------------------------
  s32 Player::getMinerals() const
  {
    return BW::PlayerMinerals->player[this->getID()];
  }
  //--------------------------------- GET GAS -----------------------------------
  s32 Player::getGas() const
  {
    return BW::PlayerGas->player[this->getID()];
  }
  //---------------------------------- GET ID -----------------------------------
  u8 Player::getID() const
  {
    return this->id;
  }
  //------------------------------- SELECTED UNIT -------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  BW::UnitData** Player::selectedUnit()
  {
    return (BW::UnitData**)(0x006284D0 + this->getID()*48);
  }
  #pragma warning(pop)
  //----------------------------- GET MINERALS LOCAL ----------------------------
  s32 Player::getMineralsLocal() const
  {
    return this->mineralsLocal;
  }
  //------------------------------- GET GAS LOCAL -------------------------------
  s32 Player::getGasLocal() const
  {
    return this->gasLocal;
  }
  //---------------------------------- UPDATE -----------------------------------
  void Player::update()
  {
    this->mineralsLocal = this->getMinerals();
    this->gasLocal = this->getGas();
  }
  //-----------------------------------------------------------------------------
  void  Player::spendLocal(s32 minerals, s32 gas)
  {
    this->mineralsLocal -= minerals;
    this->gasLocal -= gas;
  }
  //-----------------------------------------------------------------------------
};
