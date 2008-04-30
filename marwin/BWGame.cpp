#include "BWGame.h"
#include "BWHandle.h"
#include "Player.h"
#include "Unit.h"
#include "Globals.h"

#include "stdio.h"
//------------------------------- CONSTRUCTOR ----------------------------------
BWGame::BWGame(BWHandle *bwHandle)
: handleOwned(false)
 {
  this->bwHandle = bwHandle;
  this->init();
 }
//------------------------------- CONSTRUCTOR ----------------------------------
BWGame::BWGame(void)
 : handleOwned(true)
 {
  this->bwHandle = new BWHandle();
  globalBWHandle = this->bwHandle;
  globalBWGame = this;

  this->init();
 }
//-------------------------------- DESTRUCTOR ----------------------------------
BWGame::~BWGame(void)
 {
  if (this->handleOwned)
    delete this->bwHandle;
  for (int i = 0; i < playerCount; i++)
    delete players[i];
  delete [] players;
 }
//----------------------------------- INIT -------------------------------------
void BWGame::init(void)
 {
  playerCount = 12;
  players = new Player*[playerCount];
  for (int i = 0; i < playerCount; i++)
    players[i] = new Player(bwHandle, i);

  this->globalUnits = new Unit*[BW_MAX_UNIT_COUNT];
  for (int i = 0; i < BW_MAX_UNIT_COUNT; i++)
    globalUnits[i] = NULL;

  this->update();

 }
//----------------------------------- UPDATE -----------------------------------
void BWGame::update(void)
 {
  int number = 0;
  bwHandle->writeMemory((LPVOID)(0x0057F0D8 + 1*4), &number, 4);
  bwHandle->writeMemory((LPVOID)(0x0057F108 + 1*4), &number, 4);

  number = 1000;
  bwHandle->writeMemory((LPVOID)(0x0057F0D8 + 0*4), &number, 4);
  bwHandle->writeMemory((LPVOID)(0x0057F108 + 0*4), &number, 4);

  for (int i = 0; i < playerCount; i++)
   this->getPlayer(i)->update(bwHandle);
  int x,y,hp;
   byte fraction;

  Unit *unit;
  for (int i = 0; i < BW_MAX_UNIT_COUNT; i++)
  {
   x = 0; y = 0,hp = 0,fraction = 0;
   bwHandle->readMemory((LPVOID) (0x0059CB40 + 336*i + 0x18), &x, 2);
   bwHandle->readMemory((LPVOID) (0x0059CB40 + 336*i + 0x1A), &y, 2);
   bwHandle->readMemory((LPVOID) (0x0059CB40 + 336*i + 0x09), &hp, 2);
   bwHandle->readMemory((LPVOID) (0x0059CB40 + 336*i + 0x08), &fraction, 1);
   if ((x != 0 || y != 0) && (hp != 0 || fraction != 0))
    {
     if (globalUnits[i] == NULL)
      {
       unit = new Unit((LPVOID) (0x0059CB40 + 336*i));
       if (unit->playerID < playerCount)
        {
         players[unit->playerID]->addUnit(unit);
         globalUnits[i] = unit;
        }
       else
         delete unit;
      }
     else
      {
       unit = globalUnits[i];
       unit->update();
      }
    }
   else
    {
     if (globalUnits[i] != NULL)
       delete globalUnits[i];
     globalUnits[i] = NULL;
    }
   if (unit != NULL)
     unit->updateTarget();
  }
 }
//--------------------------------- GET PLAYER COUNT ---------------------------
int BWGame::getPlayerCount(void)
 {
  return playerCount;
 }
//--------------------------------- GET PLAYER ---------------------------------
Player *BWGame::getPlayer(int index)
 {
  return players[index];
 }
//------------------------------------------------------------------------------
