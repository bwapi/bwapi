#pragma once

namespace BW { struct Unit; };

#include <list>

#include <Util/Types.h>

#include <BW/TechType.h>
#include <BW/UpgradeType.h>
#include <BW/UnitType.h>
#include <BW/Offsets.h>
#include <BW/Race.h>

#include <BWAI/ReservedResources.h>

namespace BWAPI
{
  class Unit;
  /** Represents one player in game. Note that there is always player 12 who owns resources. */
  class Player
  {
  public :
    
    /**
     * Constructs new player
     * @param id 0-based Index of the player (11 for resources as it is player 12)
     */
    Player(u8 id);
    ~Player();
    /**
     * Gets the name of the player
     * @return Pointer directly to bw memory where the name is represented.
     */
    char* getName() const;
    char* getForceName() const;
    
    /** Gets local version of minerals for the current player - @ref localData */
    s32 getMinerals() const;
    /** Gets local version of gas for the current player - @ref localData */
    s32 getGas() const;
    /** Gets 0-based index of the player. (11 for neutral) */
    u8 getID() const;
    /** Modifies the local version of resources according to the specification - @ref localData */
    void spend(s32 minerals, s32 gas);
     /**
     * Gets local version of available supplies for the given race - @ref localData
     * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
     *             in that case there would be undefined behaviour.
     * @return Count of available supplies for the given race (2 times bigger than the value u see in bw)
     */                       
    s32 getSuppliesAvailable(BW::Race::Enum race);
    /**
     * Gets local version of used supplies for the given race
     * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
     *             in that case there would be undefined behaviour.
     * @return Count of used supplies for the given race (2 times bigger than the value u see in bw)
     */                       
    s32 getSuppliesUsed(BW::Race::Enum race);
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
    /** Gets the bw mapping of synchronised version of selected unit for current player */
    BW::Unit** selectedUnit();
    /**
     * Updates localData according to bw values. (Should be called every frame before local data updates are
     * performed
     */
    void update();
    /** Gets the starting race of the player. */
    BW::Race::Enum getRace();
    u8 getAlliance(u8 opposingID);
    BW::PlayerType::Enum getOwner();
    u8 getForce();
    
    s32 getAllUnits(BW::UnitType unit);
    s32 getCompletedUnits(BW::UnitType unit);
    s32 getCompletedUnits(BW::UnitType unit, BW::Race::Enum race);
    s32 getIncompleteUnits(BW::UnitType unit);
        
    s32 getDeaths(BW::UnitType unit);
    s32 getKills(BW::UnitType unit);
    void planToMake(BW::UnitType unit);
    bool researchInProgress(BW::TechType tech) const;    
    bool techResearched(BW::TechType tech) const;
    u8 upgradeLevel(BW::UpgradeType upgrade) const;
    bool upgradeInProgress(BW::UpgradeType upgrade) const; 
  private :
    /**
     * Gets synchronised version of available supplies for the given race.
     * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
     *             in that case there would be undefined behaviour.
     * @return Count of available supplies for the given race (2 times bigger than the value u see in bw)
     */                       
    s32 getSuppliesAvailableSync(BW::Race::Enum race);
    /**
     * Gets synchronised version of used supplies for the given race
     * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
     *             in that case there would be undefined behaviour.
     * @return Count of used supplies for the given race (2 times bigger than the value u see in bw)
     */                       
    s32 getSuppliesUsedSync(BW::Race::Enum race);
    /**
     * Gets synchronised version of max supplies for the given race
     * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
     *             in that case there would be undefined behaviour.
     * @return Count of maximum supplies for the given race (2 times bigger than the value u see in bw)
     */                       
    s32 getSuppliesMaxSync(BW::Race::Enum race);

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
    s32 evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit);
    s32 evaluateCounts(const BW::Counts::UnitStats& counts, BW::UnitType unit, BW::Race::Enum);
  };
};
