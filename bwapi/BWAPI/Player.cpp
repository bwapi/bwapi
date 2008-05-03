#include "Player.h"
#include "Offsets.h"
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
  return BW_PlayerNames->player[this->getID()].name;
}
//------------------------------- GET MINERALS --------------------------------
s32 Player::getMinerals() const
{
  return BW_PlayerMinerals->player[this->getID()];
}
//--------------------------------- GET GAS -----------------------------------
s32 Player::getGas() const
{
  return BW_PlayerGas->player[this->getID()];
}
//---------------------------------- GET ID -----------------------------------
u8 Player::getID() const
{
  return this->id;
}
//-----------------------------------------------------------------------------