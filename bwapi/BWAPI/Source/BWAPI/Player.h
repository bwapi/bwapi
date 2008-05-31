#pragma once

#include <list>

#include "../Types.h"

#include "../BW/UnitType.h"
#include "../BW/Offsets.h"

#include "../BW/Race.h"

namespace BW { struct Unit; };

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
    
    /**
     * Sets the name of the current player. Note that it will modify the name in bw memory, so caution needed,
     * but it didn't cause drop so far.
     * @param name New name of the player
     */
    void  setName(const char* const name);
    /** Gets synchronised version of minerals for the current player. */
    s32   getMinerals() const;
    /** Gets local version of minerals for the current player - @ref localData */
    s32   getMineralsLocal() const;
    /** Gets synchronised version of gas for the current player. */
    s32   getGas() const;
    /** Gets local version of gas for the current player - @ref localData */
    s32   getGasLocal() const;
    /** Gets 0-based index of the player. (11 for neutral) */
    u8    getID() const;
    /** Modifies the local version of resources according to the specification - @ref localData */
    void  spendLocal(s32 minerals, s32 gas);
	   /**
	    * Gets synchronised version of available supplies for the given race.
	    * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
	    *             in that case there would be undefined behaviour.
	    * @return Count of available supplies for the given race (2 times bigger than the value u see in bw)
	    */	    	    	    	       
	   s32   getSuppliesAvailable(BW::Race::Enum race);
	   /**
	    * Gets synchronised version of used supplies for the given race
	    * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
	    *             in that case there would be undefined behaviour.
	    * @return Count of used supplies for the given race (2 times bigger than the value u see in bw)
	    */	    	    	    	       
    s32   getSuppliesUsed(BW::Race::Enum race);
     /**
	    * Gets local version of available supplies for the given race - @ref localData
	    * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
	    *             in that case there would be undefined behaviour.
	    * @return Count of available supplies for the given race (2 times bigger than the value u see in bw)
	    */	    	    	    	       
    s32   getSuppliesAvailableLocal(BW::Race::Enum race);
    /**
	    * Gets local version of used supplies for the given race
	    * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
	    *             in that case there would be undefined behaviour.
	    * @return Count of used supplies for the given race (2 times bigger than the value u see in bw)
	    */	    	    	    	       
    s32   getSuppliesUsedLocal(BW::Race::Enum race);
    /**
	    * Gets local version of free supplies for the given race - @ref localData
	    * Note that free supplies are Avialable - Used.
	    * @param race Race we ask for - note that the only passable race is zerg-toss-terran not other or random
	    *             in that case there would be undefined behaviour.
	    * @return Count of available supplies for the given race (2 times bigger than the value u see in bw)
	    */	    	    	    	       
    s32   getSuppliesFreeLocal(BW::Race::Enum race);
    /**
     * Order to use (reserve) certain amount of supplies in the local version - @ref localData
     * @param supplies Amount of supplies to reserve.
     * @param race Race we want to affect.
     */
    void  useSuppliesLocal(u8 supplies, BW::Race::Enum race);
    /** Gets the bw mapping of synchronised version of selected unit for current player */
    BW::Unit** selectedUnit();
    /**
     * Updates localData according to bw values. (Should be called every frame before local data updates are
     * performed
     */
    void update();
    /**
     * Checks unit availability for current player, could be probably solved by reading something in memory
     * but we didn't find the address so far, so we have to write the logic on our own.
     */
    bool canBuild(BW::UnitType unit);
    /** Count of units of the same type */
    u16 unitTypeCount[BW::unitTypeCount];
    /**
     * Finds out if the current player can afford to build the specified unit atm Checks minerals, gas and
     * supply need
     * @param unit Unit type to be checked
     * @param Money that must the player keep (reserved to make building or other reason)
     * @returns @c true if the unit can be trained, @c false otherwise.
     */
    bool canAfford(BW::UnitType unit, u16 mineralsToKeep = 0);
  private :
    u8 id;  /**< Order of the player, is used to load player's information from the memory */
    s32 mineralsLocal, gasLocal;  /**< Storage of local versions resources. */

    s32 suppliesAvailableLocal[BW::RaceCount]; /**< Storage of local versions of Available supplies. */
    s32 suppliesUsedLocal[BW::RaceCount]; /**< Storage of local versions of used supplies. */
  };
};
