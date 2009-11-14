#pragma once

namespace BW { struct Unit; };

#include <BWAPI/Race.h>
#include <BWAPI/Player.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/UnitType.h>
#include "UnitImpl.h"
#include "ForceImpl.h"

#include <list>
#include <set>
#include <string>
#include <Util/Types.h>

#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/UnitType.h>
#include <BW/Offsets.h>
#include <BW/Race.h>
#include <BW/TilePosition.h>

namespace BWAPI
{
  class Unit;
  class Force;
  /** Represents one player in game. Note that there is always player 12 who owns resources. */
  class PlayerImpl : public Player
  {
    public :

      /**
       * Gets the name of the player
       * @return Pointer directly to bw memory where the name is represented.
       */
      virtual int __cdecl getID() const;
      virtual std::string __cdecl getName() const;
      virtual std::set<Unit*> __cdecl getUnits();
      virtual Race __cdecl getRace() const;
      virtual PlayerType __cdecl playerType() const;
      virtual Force* __cdecl getForce() const;
      virtual bool __cdecl isAlly(Player* player) const;
      virtual bool __cdecl isEnemy(Player* player) const;
      virtual bool __cdecl isNeutral() const;
      virtual TilePosition __cdecl getStartLocation() const;

      virtual int __cdecl minerals() const;
      virtual int __cdecl gas() const;

      virtual int __cdecl cumulativeMinerals() const;
      virtual int __cdecl cumulativeGas() const;

      virtual int __cdecl supplyTotal() const;
      virtual int __cdecl supplyUsed() const;
      virtual int __cdecl supplyTotal(Race race) const;
      virtual int __cdecl supplyUsed(Race race) const;

      virtual int __cdecl allUnitCount(UnitType unit) const;
      virtual int __cdecl completedUnitCount(UnitType unit) const;
      virtual int __cdecl incompleteUnitCount(UnitType unit) const;
      virtual int __cdecl deadUnitCount(UnitType unit) const;
      virtual int __cdecl killedUnitCount(UnitType unit) const;

      virtual bool __cdecl researching(TechType tech) const;
      virtual bool __cdecl researched(TechType tech) const;
      virtual bool __cdecl upgrading(UpgradeType upgrade) const;
      virtual int __cdecl upgradeLevel(UpgradeType upgrade) const;

      //Internal BWAPI commands:

      /**
       * Constructs new player
       * @param id 0-based Index of the player (11 for resources as it is player 12)
       */
      PlayerImpl(u8 id);
      ~PlayerImpl();
      /** Gets 0-based index of the player. (11 for neutral) */
      char* getForceName() const;
      u8 getAlliance(u8 opposingID);
      u8 getForce();
      ForceImpl* force;

      /** Modifies the local version of resources according to the specification - @ref localData */
      void spend(s32 minerals, s32 gas);

      /** Gets the bw mapping of synchronised version of selected unit for current player */
      BW::Unit** selectedUnit();
      /**
       * Updates localData according to bw values. (Should be called every frame before local data updates are
       * performed
       */
      void update();
      UnitImpl* getFirst();

      void planToMake(BW::UnitType unit);
      /**
       * Gets local version of free supplies for the given race - @ref localData
       * Note that free supplies are Avialable - Used.
       * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
       *             in that case there would be undefined behaviour.
       * @return Count of available supplies for the given race (2 times bigger than the value u see in bw)
       */
      s32 getSuppliesFree(BW::Race::Enum race);
      /**
       * Order to use (reserve) certain amount of supplies in the local version - @ref localData
       * @param supplies Amount of supplies to reserve.
       * @param race Race we want to affect.
       */
      void useSupplies(u8 supplies, BW::Race::Enum race);
      void onGameEnd();
    private :
      /**
       * Gets synchronised version of available supplies for the given race.
       * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
       *             in that case there would be undefined behaviour.
       * @return Count of available supplies for the given race (2 times bigger than the value u see in bw)
       */
      s32 getSuppliesAvailableSync(BW::Race::Enum race) const;
      /**
       * Gets synchronised version of used supplies for the given race
       * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
       *             in that case there would be undefined behaviour.
       * @return Count of used supplies for the given race (2 times bigger than the value u see in bw)
       */
      s32 getSuppliesUsedSync(BW::Race::Enum race) const;
      /**
       * Gets synchronised version of max supplies for the given race
       * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
       *             in that case there would be undefined behaviour.
       * @return Count of maximum supplies for the given race (2 times bigger than the value u see in bw)
       */
      s32 getSuppliesMaxSync(BW::Race::Enum race) const;

      /** Gets synchronised version of minerals for the current player. */
      s32 getMineralsSync() const;
      /** Gets synchronised version of gas for the current player. */
      s32 getGasSync() const;

      u8 id;  /**< Order of the player, is used to load player's information from the memory */
      s32 mineralsLocal, gasLocal;  /**< Storage of local versions resources. */

      s32 toMake[BW::UNIT_TYPE_COUNT]; /**< Structure usxed to store datea needed to get local version of counts
                                      * of units */
      s32 suppliesAvailableLocal[BW::RACE_COUNT]; /**< Storage of local versions of Available supplies. */
      s32 suppliesUsedLocal[BW::RACE_COUNT]; /**< Storage of local versions of used supplies. */
      s32 evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit) const;

      std::set<Unit*> units;
      int unitCacheFrame;
  };
};
