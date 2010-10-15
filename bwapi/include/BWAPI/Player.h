#pragma once
#include <set>
#include <string>

#include <BWAPI/TilePosition.h>
#include <BWAPI/Race.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/Client/PlayerData.h>
namespace BWAPI
{
  class Unit;
  class Force;

  /** Each player in a match will have his or her own player instance. There is also a neutral player which
   * owns all the neutral units. */
  class Player
  {
    public :
      /** Returns a unique ID for the player. */
      virtual int getID() const = 0;

      /** Returns the name of the player. */
      virtual std::string getName() const = 0;

      /** Returns the set of units the player own. Note that units loaded into Terran dropships, Terran
       * bunkers, Terran refineries, Protoss assimilators, and Zerg extractors are not yet included in the
       * set. */
      virtual const std::set<Unit*>& getUnits() const = 0;

      /** Returns the race of the player. */
      virtual Race getRace() const = 0;

      /** Returns the type of the player. */
      virtual PlayerType getType() const = 0;

      /** Returns the force the player is on. */
      virtual Force* getForce() const = 0;

      /** Returns true if other player is an ally of this player. */
      virtual bool isAlly(Player* player) const = 0;

      /** Returns true if other player is an enemy of this player. */
      virtual bool isEnemy(Player* player) const = 0;

      /** Returns true if the player is the neutral player. */
      virtual bool isNeutral() const = 0;

      /** Returns the starting location of the player. If complete map information is disabled, this function
       * will return TilePositions::Unknown for enemy players. For the complete set of starting locations for
       * the current map, see Game::getStartLocations. */
      virtual TilePosition getStartLocation() const = 0;

      /** Returns true if the player has achieved victory. */
      virtual bool isVictorious() const = 0;

      /** Returns true if the player has been defeated. */
      virtual bool isDefeated() const = 0;

      /** Returns true if the player left the game. */
      virtual bool leftGame() const = 0;

      /** Returns the amount of minerals the player has. */
      virtual int minerals() const = 0;

      /** Returns the amount of vespene gas the player has. */
      virtual int gas() const = 0;

      /** Returns the cumulative amount of minerals the player has mined up to this point (including the 50
       * minerals at the start of the game). */
      virtual int cumulativeMinerals() const = 0;

      /** Returns the cumulative amount of gas the player has harvested up to this point. */
      virtual int cumulativeGas() const = 0;

      // TODO: ground methods
      /** Returns the total amount of supply the player has. If a race is provided, the total supply for the
       * given race will be returned, otherwise the player's initial race will be used. Supply counts returned
       * by BWAPI are double what you would expect to see from playing the game. This is because zerglings
       * take up 0.5 in-game supply. */
      virtual int supplyTotal() const = 0;
      virtual int supplyTotal(Race race) const = 0;

      /** Returns how much of the supply is actually being used by units. If a race is provided, the used
       * supply for the given race will be returned, otherwise the player's initial race will be used. Supply
       * counts returned by BWAPI are double what you would expect to see from playing the game. This is
       * because zerglings take up 0.5 in-game supply. */
      virtual int supplyUsed() const = 0;
      virtual int supplyUsed(Race race) const = 0;

      /** Returns the number of all units of the given type. */
      virtual int allUnitCount(UnitType unit) const = 0;

      /** Returns the number of completed units of the given type. */
      virtual int completedUnitCount(UnitType unit) const = 0;

      /** Returns the number of incomplete units of the given type. */
      virtual int incompleteUnitCount(UnitType unit) const = 0;

      /** Returns the number of dead units of the given type. */
      virtual int deadUnitCount(UnitType unit) const = 0;

      /** Returns the number of killed units of the given type. */
      virtual int killedUnitCount(UnitType unit) const = 0;

      /** Returns the player's current upgrade level of the given upgrade. To order a unit to upgrade a given
       * upgrade type, see Unit::upgrade. */
      virtual int  getUpgradeLevel(UpgradeType upgrade) const = 0;

      /** Returns true if the player has finished researching the given tech. To order a unit to research a
       * given tech type, see Unit::research. */
      virtual bool hasResearched(TechType tech) const = 0;

      /** Returns true if the player is researching the given tech. To order a unit to research a given tech
       * type, see Unit::reseach. */
      virtual bool isResearching(TechType tech) const = 0;

      /** Returns true if the player is upgrading the given upgrade. To order a unit to upgrade a given
       * upgrade type, see Unit::upgrade. */
      virtual bool isUpgrading(UpgradeType upgrade) const = 0;

      /** Returns the max energy of the given unit type, taking into account upgrades */
      virtual int maxEnergy(UnitType unit) const = 0;
  };
};
