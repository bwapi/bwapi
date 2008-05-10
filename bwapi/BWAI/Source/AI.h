#pragma once
#include "..//..//BWAPI//Source//BWAPI//Game.h"
//#include "..//..//BWAPI//Source//BW//UnitData.h"
#include <vector>

namespace BWAI { class Unit; }
namespace BWAI { class Mineral; }
namespace BWAI { class Expansion; }

namespace BWAPI { class Player; }
namespace BWAPI { class UnitPrototype; }

/** 
 * Set of classes providing ai tools.
 */
namespace BWAI
{
  class AI
  {
    public :
      AI();
      /** Call in the start of the game */
      void AI::onStart(BWAPI::Game& game, BWAPI::Player *player);
      void onFrame(BWAPI::Game& game);
      int suppliesOrdered;
      std::vector<Expansion*> expansions;
      std::vector<Mineral*> activeMinerals;
      Unit* getUnit(int index);
   private :
      BWAPI::Player* player;
      Unit* units[BW::UNIT_ARRAY_MAX_LENGTH];
      BWAPI::UnitPrototype* worker;

      //static BWAI* ;
  };
}
