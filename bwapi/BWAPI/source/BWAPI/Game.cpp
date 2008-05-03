#include "Game.h"

#include "../BWAPI/Player.h"
#include "../BWAPI/Unit.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"

#include "../BW/Offsets.h"
#include "../BW/UnitData.h"

#include <stdio.h>

namespace BWAPI 
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Game::Game()
  {
    unitArrayCopy = new BW::UnitArray;

    for (int i = 0; i < 12; i++)
      players[i] = new Player(i);    
    
    players[11]->setName("Player 12 (Neutral)");
    
    for (int i = 0; i < 1700; i++)
      units[i] = new Unit(&unitArrayCopy->unit[i]);

    this->update();
  }
  //------------------------------- DESTRUCTOR ----------------------------------
  Game::~Game()
  {
    delete unitArrayCopy;

    for (int i = 0; i < 12; i++)
      delete players[i];

    for (int i = 0; i < 1700; i++)
      delete units[i];
  }
  int test = 0;
  //---------------------------------- UPDATE -----------------------------------
  void Game::update()
  {
    memcpy(this->unitArrayCopy, BW::UnitNodeTable, sizeof(BW::UnitArray));
    players[11]->setName("Player 12 (Neutral)");
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
    f = fopen("bwapi.log","at"); 
    fprintf(f, "Found units :\n");
    bool found = false;
    for (int i = 0; i < 1700; i++)
    {
      if (units[i]->isValid() && 
          units[i]->getPrototype() != NULL &&
          strcmp(units[i]->getOwner()->getName(),"Marwin") == 0)
      {
        fprintf(f, "(%s) (%d,%d) (%s)\n", 
          units[i]->getPrototype()->getName().c_str(),
          units[i]->getPosition().x,
          units[i]->getPosition().y,
          units[i]->getOwner()->getName());
        found = true;
      }
    }
   fclose(f);
   
   if (found)
   { 
     test ++;
     BW::Position target;
     target.x = 0;
     target.y = 0;
     units[0]->order(test,target);            
   }
   
  }
  //-----------------------------------------------------------------------------
};