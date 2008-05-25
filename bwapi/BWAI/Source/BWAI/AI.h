#pragma once
#include "../../BWAPI/Source/BWAPI/Game.h"
#include <list>

namespace Util { class Logger; }
namespace BWAI { class Unit; }
namespace BWAI { class TaskGather; }
namespace BWAI { class TaskBuild; }
namespace BWAI { class Expansion; }
namespace BWAI { class MapInfo; }
namespace BWAI { class MapStartingPosition; }
namespace BWAI { class BuildingToMake; }

namespace BuildOrder { class Root; }

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
      ~AI();
      /** Updates next/first pointers */
      void update();
      /** Call in the start of the game */
      void onStart(BWAPI::Player *player);
      void onEnd();
      void onFrame();
      void onCancelTrain();
      void onRemoveUnit(BW::Unit* unit);

      std::list<Expansion*> expansions;
      Unit* getUnit(int index);
      static Unit* optimizeMineralFor;
      int expansionsSaturated;
      Unit* getFirst();
      Util::Logger* log;
      Unit* freeBuilder(BW::Position position);
      
      std::list<TaskGather*> activeMinerals;
      std::list<TaskBuild*> plannedBuildings;
      BuildOrder::Root *root;
   private :
      BWAPI::Player* player;
      Unit* units[BW::UNIT_ARRAY_MAX_LENGTH];
      BWAPI::UnitPrototype* worker;
      Unit* first;
      Util::Logger* deadLog;      
      MapInfo *mapInfo;
      MapStartingPosition* startingPosition;
      
      void startNewExpansion(Unit *gatherCenter);
      void rebalanceMiners();
      void checkAssignedWorkers();
      void checkNewExpansions();
      void refreshSelectionStates(BW::Unit** selected);
      /** 
       * If some building finished production, it will start the same unit again
       * @returns something happened (so reselect is needed)
       */
      bool performAutoBuild();
      void getIdleWorkers(std::list<Unit*> &workers);
      void checkWorkersNeed();
      void assignIdleWorkersToMinerals(std::list<Unit*>& idleWorkers);
      int countOfTerranProductionBuildings();
      void checkSupplyNeed();
      /** @todo investigate and use the nextSupply provider here. */ 
      s32 plannedTerranSupplyGain();
      void executeTasks();
      TaskGather* bestFor(Unit* gatherer);
      void removeExpansion(Expansion* expansion);
  };
}
