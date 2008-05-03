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
  //-----------------------------------------------------------------------------
};
