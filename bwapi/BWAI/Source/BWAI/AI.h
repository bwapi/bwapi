#pragma once

#include <list>
#include <map>

#include "../../../BWAPI/Source/BWAPI/Game.h"
#include "../../../BWAPI/Source/BW/UnitType.h"

namespace Util { class Logger; }
namespace BWAI { class Unit; }
namespace BWAI { class TaskGatherGas; }
namespace BWAI { class TaskGather; }
namespace BWAI { class TaskBuild; }
namespace BWAI { class Expansion; }
namespace BWAI { class MapInfo; }
namespace BWAI { class MapStartingPosition; }
namespace BWAI { class BuildingPosition; }

namespace BuildOrder { class Root; }
namespace BuildOrder { class Command; }
namespace BuildOrder { class Branch; }

namespace BWAPI { class Player; }
namespace BWAPI { class UnitPrototype; }
namespace BWAPI { class Map; }

/** 
 * Set of classes providing ai tools.
 */
namespace BWAI
{
  /**
   * Singleton - root ai class maintaining all the stuff, it's only instance
   * should be the one defined in globals. 
   *
   * There are good reasons that the class is constructed dynamically, the main
   * is that non-dynamical classes have not defined order of constructor (c++
   * definition), so this class is constructated manually justa after the 
   * #BWAPI#Game is constructed and it uses it's content (configuration, logs)
   */
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
      bool expansionsSaturated;
      Unit* getFirst();
      Util::Logger* log;
      Unit* freeBuilder(BW::Position position);

      std::list<TaskGatherGas*> activeRefineries;      
      std::list<TaskGather*> activeMinerals;
      std::list<TaskBuild*> plannedBuildings;
      BuildOrder::Root *root;
      std::list<BuildOrder::Command*>::iterator actualPosition;
      BuildOrder::Branch* actualBranch;
      
      std::map<std::string, BW::UnitType> unitNameToType;
      BWAPI::Player* player;
      BW::TilePosition getFreeBuildingSpot(std::string spotName, Unit*& builderToUse);
      /**
       * Gets set of building positions (wrapped by the BuildingPosition class)
       * with the specified id. The operation speed should be log(count of
       * places) as the stl doesn't support hash, but whatever.
       * @param name of the building-place-set identification - it is the same
       *        as the name attribute in <building> tak in map xml file.
       * @returns BuildingPosition set with he specified name if present, @c
       *          NULL otherwise.
       */
      BuildingPosition* getPositionsCalled(const std::string& place);
      u16 moneyToBeSpentOnBuildings;
      /** Currently opened map, NULL if no map is opened. */
      BWAPI::Map* map;
   private :
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
      void unsaturateGather();
      void removeGatherGasTask(Unit* refinery);
  };
}
