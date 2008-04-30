#include "Player.h"
#include "Unit.h"
#include <stdio.h>
#include "BWHandle.h"
//------------------------------- CONSTRUCTOR ----------------------------------
Player::Player(BWHandle *bwHandle, int playerIndex)
 : minerals(0),
   gas(0),
   playerIndex(playerIndex)
 {
  this->units = NULL;
 }
//------------------------------- DESTRUCTOR ----------------------------------
Player::~Player(void)
 {
  Unit *temp = units;
  while (temp!= NULL)
   {
    units = temp;
    temp = temp->next;
    delete units;
   }
 }
//------------------------------------------------------------------------------
void Player::update(BWHandle *bwHandle)
 {
  bwHandle->readMemory((LPVOID)(0x0057F0D8 + playerIndex*4), &minerals, 4);
  bwHandle->readMemory((LPVOID)(0x0057F108 + playerIndex*4), &gas, 4);
  if (units != NULL)
    units->update();
 }
//------------------------------------------------------------------------------
void Player::addUnit(Unit* unit)
 {
  unit->next = units;
  if (units != NULL)
    units->previous = unit;
  units = unit;
 }
//------------------------------------------------------------------------------
