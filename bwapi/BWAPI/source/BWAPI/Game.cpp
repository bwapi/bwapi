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
  //---------------------------------- UPDATE -----------------------------------
  int test = 0;
  void Game::update()
  {
    memcpy(this->unitArrayCopy, BW::UnitNodeTable, sizeof(BW::UnitArray));
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
    {
      if (units[i]->isValid() && NULL != units[i]->getPrototype())
      {
        fprintf(f, "(%s) (%d,%d) \n", 
          units[i]->getPrototype()->getName().c_str(),
          units[i]->getPosition().x,
          units[i]->getPosition().y);
        BW::Position target;
        target.x = 100;
        target.y = 100;
        units[i]->order(test, target);
        found = true;
      }
      if (found)
        test ++;
      fclose(f);
    }

  }
  //-----------------------------------------------------------------------------
};