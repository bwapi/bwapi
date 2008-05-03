#include "Game.h"
#include "Player.h"
#include "Offsets.h"
#include "Unit.h"
#include "UnitPrototype.h"
#include "UnitPrototypeDefinitions.h"

#include <stdio.h>

//------------------------------ CONSTRUCTOR ----------------------------------
Game::Game()
{
  for (int i = 0; i < 12; i++)
    players[i] = new Player(i);

  unitArrayCopy = new BW_UnitArray;

  
  for (int i = 0; i < 1700; i++)
     units[i] = new Unit(&unitArrayCopy->Unit[i]);
  this->update();
}
//------------------------------- DESTRUCTOR ----------------------------------
Game::~Game()
{
 for (int i = 0; i < 12; i++)
    delete players[i];
  delete unitArrayCopy;
}
//---------------------------------- UPDATE -----------------------------------
int test = 0;
void Game::update()
{
  memcpy(this->unitArrayCopy, UNIT_NODE_TABLE, sizeof(BW_UnitArray));
  /*
  I will implement this later on using some correct pointers method on unit
  _w64 int memoryPositionDifference = this->unitArrayCopy - UNIT_NODE_TABLE; 
  for (int i = 0; i < 1700; i++)
  {
     units[i]->rawData->previousUnit += memoryPositionDifference;
     units[i]->rawData->nextUnit += memoryPositionDifference;
  }
  */
  FILE *f;
  f = fopen("bwapi.log","wt"); 
  fprintf(f, "Found units :\n");
  fprintf(f, "Command Id = %d\n", test);
  bool found = false;
  for (int i = 0; i < 1700; i++)
     if (units[i]->isValid() &&
         units[i]->getPrototype() != 0)
     {
        fprintf(f, "(%s) (%d,%d) \n", units[i]->getPrototype()->getName().c_str(),
                                      units[i]->getPosition().x,
                                      units[i]->getPosition().y);
        BW_Position target;
        target.x = 100;
        target.y = 100;
        units[i]->order(test, target);
        found = true;
     }
  if (found)
     test ++;
  fclose(f);
   
}
//-----------------------------------------------------------------------------