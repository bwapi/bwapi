#pragma once

#include <string_view>

#include "Client/PlayerData.h"

#include <BWAPI/Position.h>
#include <BWAPI/Race.h>
#include <BWAPI/UnitType.h>

namespace BWAPI
{
  // Forwards
  class Color;
  class ForceInterface;
  typedef ForceInterface *Force;
  class PlayerType;
  class TechType;
  class Unitset;
  class UpgradeType;
  class WeaponType;
  class Game;

  struct PlayerData;

  /// <summary>The Player represents a unique controller in the game.</summary> Each player in
  /// a match will have his or her own player instance. There is also a neutral player which owns
  /// all the neutral units (such as mineral patches and vespene geysers).
  ///
  /// @see Playerset, PlayerType, Race
  /// @ingroup Interface
  class Player: public PlayerID {
    std::reference_wrapper<Game> game;
  public:
    Player(Game &game, PlayerID id): game{game}, PlayerID{id} { }

    Game &getGame() const { return game; }

    PlayerData &getPlayerData() const;

    /// <summary>Retrieves the name of the player.</summary>
    ///
    /// @returns
    ///   A std::string object containing the player's name.
    ///
    /// @note Don't forget to use std::string::c_str() when passing this parameter to
    /// Game::sendText and other variadic functions.
    ///
    /// Example usage:
    /// @code
    ///   BWAPI::Player myEnemy = BWAPI::Broodwar->enemy();
    ///   if ( myEnemy != nullptr )   // Make sure there is an enemy!
    ///     BWAPI::Broodwar->sendText("Prepare to be crushed, %s!", myEnemy->getName().c_str());
    /// @endcode
    std::string_view getName() const { return getPlayerData().name; }

    /// <summary>Retrieves the set of all units that the player owns.</summary> This also includes
    /// incomplete units.
    ///
    /// @returns Reference to a Unitset containing the units.
    ///
    /// @note This does not include units that are loaded into transports, @Bunkers, @Refineries,
    /// @Assimilators, or @Extractors.
    ///
    /// Example usage:
    /// @code
    ///   Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
    ///   for ( auto u = myUnits.begin(); u != myUnits.end(); ++u )
    ///   {
    ///     // Do something with your units
    ///   }
    /// @endcode
    Unitset &getUnits() const;

    /// <summary>Retrieves the race of the player.</summary> This allows you to change strategies
    /// against different races, or generalize some commands for yourself.
    ///
    /// @retval Races::Unknown If the player chose Races::Random when the game started and they
    /// have not been seen.
    ///
    /// @returns
    ///   The Race that the player is using.
    ///
    /// Example usage:
    /// @code
    ///   if ( BWAPI::Broodwar->enemy() )
    ///   {
    ///     BWAPI::Race enemyRace = BWAPI::Broodwar->enemy()->getRace();
    ///     if ( enemyRace == Races::Zerg )
    ///       BWAPI::Broodwar->sendText("Do you really think you can beat me with a zergling rush?");
    ///   }
    /// @endcode
    Race getRace() const { return getPlayerData().race; }

    /// <summary>Retrieves the player's controller type.</summary> This allows you to distinguish
    /// betweeen computer and human players.
    ///
    /// @returns
    ///   The PlayerType that identifies who is controlling a player.
    ///
    /// @note Other players using BWAPI will be treated as a human player and return
    /// PlayerTypes::Player.
    ///
    /// @code
    ///   if ( BWAPI::Broodwar->enemy() )
    ///   {
    ///     if ( BWAPI::Broodwar->enemy()->getType() == PlayerTypes::Computer )
    ///       BWAPI::Broodwar << "Looks like something I can abuse!" << std::endl;
    ///   }
    /// @endcode
    PlayerType getType() const { return getPlayerData().type; }

    /// <summary>Retrieves the player's force.</summary> A force is the team that the player is
    /// playing on.
    ///
    /// @returns
    ///   The Force object that the player is part of.
    Force getForce() const { return getPlayerData().force; }

    /// <summary>Checks if this player is allied to the specified player.</summary>
    ///
    /// <param name="otherPlayer">
    ///     The player to check alliance with.
    /// </param>
    ///
    /// @retval true if this player is allied with \p player .
    /// @retval false if this player is not allied with \p player.
    ///
    /// @note This function will also return false if this player is neutral or an observer, or
    /// if \p otherPlayer is neutral or an observer.
    ///
    /// @see isEnemy
    bool isAlly(Player otherPlayer) const {
      if (!otherPlayer || isNeutral() || otherPlayer.isNeutral())
        return false;
      return getPlayerData().isAlly[static_cast<int>(otherPlayer.getID())];
    }

    /// <summary>Checks if this player is unallied to the specified player.</summary>
    ///
    /// <param name="otherPlayer">
    ///     The player to check alliance with.
    /// </param>
    ///
    /// @retval true if this player is allied with \p player .
    /// @retval false if this player is not allied with \p player .
    ///
    /// @note This function will also return false if this player is neutral or an observer, or if
    /// \p otherPlayer is neutral or an observer.
    ///
    /// @see isAlly
    bool isEnemy(Player otherPlayer) const {
      if (!otherPlayer || isNeutral() || otherPlayer.isNeutral())
        return false;
      return getPlayerData().isEnemy[static_cast<int>(otherPlayer.getID())];
    }

    /// <summary>Checks if this player is the neutral player.</summary>
    ///
    /// @retval true if this player is the neutral player.
    /// @retval false if this player is any other player.
    bool isNeutral() const { return getPlayerData().isNeutral; }

    /// <summary>Retrieve's the player's starting location.</summary>
    ///
    /// @returns
    ///   A TilePosition containing the position of the start location.
    ///
    /// @retval TilePositions::None if the player does not have a start location.
    /// @retval TilePositions::Unknown if an error occured while trying to retrieve the start
    /// location.
    ///
    /// @see Game::getStartLocations, Game::getLastError
    TilePosition getStartLocation() const { return { getPlayerData().startLocationX, getPlayerData().startLocationY }; }

    /// <summary>Checks if the player has achieved victory.</summary>
    ///
    /// @returns true if this player has achieved victory, otherwise false
    bool isVictorious() const { return getPlayerData().isVictorious; }

    /// <summary>Checks if the player has been defeated.</summary>
    ///
    /// @returns true if the player is defeated, otherwise false
    bool isDefeated() const { return getPlayerData().isDefeated; }

    /// <summary>Checks if the player has left the game.</summary>
    ///
    /// @returns true if the player has left the game, otherwise false
    bool leftGame() const { return getPlayerData().leftGame; }

    /// <summary>Retrieves the current amount of minerals/ore that this player has.</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Amount of minerals that the player currently has for spending.
    int minerals() const { return getPlayerData().minerals; }

    /// <summary>Retrieves the current amount of vespene gas that this player has.</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Amount of gas that the player currently has for spending.
    int gas() const { return getPlayerData().gas; }

    /// <summary>Retrieves the cumulative amount of minerals/ore that this player has gathered
    /// since the beginning of the game, including the amount that the player starts the game
    /// with (if any).</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of minerals that the player has gathered.
    int gatheredMinerals() const { return getPlayerData().gatheredMinerals; }

    /// <summary>Retrieves the cumulative amount of vespene gas that this player has gathered since
    /// the beginning of the game, including the amount that the player starts the game with (if
    /// any).</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of gas that the player has gathered.
    int gatheredGas() const { return getPlayerData().gatheredGas; }

    /// <summary>Retrieves the cumulative amount of minerals/ore that this player has spent on
    /// repairing units since the beginning of the game.</summary> This function only applies to
    /// @Terran players.
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of minerals that the player has spent repairing.
    int repairedMinerals() const { return getPlayerData().repairedMinerals; }

    /// <summary>Retrieves the cumulative amount of vespene gas that this player has spent on
    /// repairing units since the beginning of the game.</summary> This function only applies to
    /// @Terran players.
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of gas that the player has spent repairing.
    int repairedGas() const { return getPlayerData().repairedGas; }

    /// <summary>Retrieves the cumulative amount of minerals/ore that this player has gained from
    /// refunding (cancelling) units and structures.</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of minerals that the player has received from refunds.
    int refundedMinerals() const { return getPlayerData().refundedMinerals; }

    /// <summary>Retrieves the cumulative amount of vespene gas that this player has gained from
    /// refunding (cancelling) units and structures.</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of gas that the player has received from refunds.
    int refundedGas() const { return getPlayerData().refundedGas; }

    /// <summary>Retrieves the cumulative amount of minerals/ore that this player has spent,
    /// excluding repairs.</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of minerals that the player has spent.
    int spentMinerals() const { return gatheredMinerals() + refundedMinerals() - minerals() - repairedMinerals(); }

    /// <summary>Retrieves the cumulative amount of vespene gas that this player has spent,
    /// excluding repairs.</summary>
    ///
    /// @note This function will return 0 if the player is inaccessible.
    ///
    /// @returns Cumulative amount of gas that the player has spent.
    int spentGas() const { return gatheredGas() + refundedGas() - gas() - repairedGas(); }

    /// <summary>Retrieves the total amount of supply the player has available for unit control.</summary>
    ///
    /// @note In Starcraft programming, the managed supply values are double than what they appear
    /// in the game. The reason for this is because @Zerglings use 0.5 visible supply.
    ///
    /// @note In Starcraft, the supply for each race is separate. Having a @Pylon and an @Overlord
    /// will not give you 32 supply. It will instead give you 16 @Protoss supply and 16 @Zerg
    /// supply.
    ///
    /// <param name="race"> (optional)
    ///   The race to query the total supply for. If this is omitted, then the player's current
    ///   race will be used.
    /// </param>
    ///
    /// @returns The total supply available for this player and the given \p race.
    ///
    /// Example usage:
    /// @code
    ///   if ( BWAPI::Broodwar->self()->supplyUsed() + 8 >= BWAPI::Broodwar->self()->supplyTotal() )
    ///   {
    ///     // Construct pylons, supply depots, or overlords
    ///   }
    /// @endcode
    /// @see supplyUsed
    int supplyTotal(Race race = Races::None) const {
      auto const irace = static_cast<int>(race);
      return 0 <= irace && irace < 3 ? getPlayerData().supplyTotal[irace] : 0;
    }

    /// <summary>Retrieves the current amount of supply that the player is using for unit control.</summary>
    ///
    /// <param name="race"> (optional)
    ///   The race to query the used supply for. If this is omitted, then the player's current
    ///   race will be used.
    /// </param>
    ///
    /// @returns The supply that is in use for this player and the given \p race.
    /// @see supplyTotal
    int supplyUsed(Race race = Races::None) const {
      auto const irace = static_cast<int>(race);
      return 0 <= irace && irace < 3 ? getPlayerData().supplyUsed[irace] : 0;
    }

    /// <summary>Retrieves the total number of units that the player has.</summary> If the
    /// information about the player is limited, then this function will only return the number
    /// of visible units.
    ///
    /// @note While in-progress @Protoss and @Terran units will be counted, in-progress @Zerg units
    /// (i.e. inside of an egg) do not.
    ///
    /// <param name="unit"> (optional)
    ///   The unit type to query. UnitType macros are accepted. If this parameter is omitted,
    ///   then it will use UnitTypes::AllUnits by default.
    /// </param>
    ///
    /// @returns The total number of units of the given type that the player owns.
    /// @see visibleUnitCount, completedUnitCount, incompleteUnitCount
    int allUnitCount(UnitType unit = UnitTypes::AllUnits) const {
      return unit ? getPlayerData().allUnitCount[static_cast<int>(unit)] : 0;
    }

    /// <summary>Retrieves the total number of strictly visible units that the player has, even if
    /// information on the player is unrestricted.</summary>
    ///
    /// <param name="unit"> (optional)
    ///   The unit type to query. UnitType macros are accepted. If this parameter is omitted,
    ///   then it will use UnitTypes::AllUnits by default.
    /// </param>
    ///
    /// @returns The total number of units of the given type that the player owns, and is visible
    ///   to the BWAPI player.
    /// @see allUnitCount, completedUnitCount, incompleteUnitCount
    int visibleUnitCount(UnitType unit = UnitTypes::AllUnits) const {
      return unit ? getPlayerData().visibleUnitCount[static_cast<int>(unit)] : 0;
    }

    /// <summary>Retrieves the number of completed units that the player has.</summary> If the
    /// information about the player is limited, then this function will only return the number of
    /// visible completed units.
    ///
    /// <param name="unit"> (optional)
    ///   The unit type to query. UnitType macros are accepted. If this parameter is omitted,
    ///   then it will use UnitTypes::AllUnits by default.
    /// </param>
    ///
    /// @returns The number of completed units of the given type that the player owns.
    ///
    /// Example usage:
    /// @code
    ///   bool obtainNextUpgrade(BWAPI::UpgradeType upgType)
    ///   {
    ///     BWAPI::Player self = BWAPI::Broodwar->self();
    ///     int maxLvl      = self->getMaxUpgradeLevel(upgType);
    ///     int currentLvl  = self->getUpgradeLevel(upgType);
    ///     if ( !self->isUpgrading(upgType) && currentLvl < maxLvl &&
    ///           self->completedUnitCount(upgType.whatsRequired(currentLvl+1)) > 0 &&
    ///           self->completedUnitCount(upgType.whatUpgrades()) > 0 )
    ///       return self->getUnits().upgrade(upgType);
    ///     return false;
    ///   }
    /// @endcode
    ///
    /// @see allUnitCount, visibleUnitCount, incompleteUnitCount
    int completedUnitCount(UnitType unit = UnitTypes::AllUnits) const {
      return unit ? getPlayerData().completedUnitCount[static_cast<int>(unit)] : 0;
    }

    /// <summary>Retrieves the number of incomplete units that the player has.</summary> If the
    /// information about the player is limited, then this function will only return the number of
    /// visible incomplete units.
    ///
    /// @note This function is a macro for allUnitCount() - completedUnitCount().
    ///
    /// @note Incomplete @Zerg units inside of eggs are not counted.
    ///
    /// <param name="unit"> (optional)
    ///   The unit type to query. UnitType macros are accepted. If this parameter is omitted,
    ///   then it will use UnitTypes::AllUnits by default.
    /// </param>
    ///
    /// @returns The number of incomplete units of the given type that the player owns.
    /// @see allUnitCount, visibleUnitCount, completedUnitCount
    int incompleteUnitCount(UnitType unit = UnitTypes::AllUnits) const {
      return unit ? allUnitCount(unit) - completedUnitCount(unit) : 0;
    }

    /// <summary>Retrieves the number units that have died for this player.</summary>
    ///
    /// <param name="unit"> (optional)
    ///   The unit type to query. UnitType macros are accepted. If this parameter is omitted,
    ///   then it will use UnitTypes::AllUnits by default.
    /// </param>
    ///
    /// @returns The total number of units that have died throughout the game.
    int deadUnitCount(UnitType unit = UnitTypes::AllUnits) const {
      return unit ? getPlayerData().deadUnitCount[static_cast<int>(unit)] : 0;
    }

    /// <summary>Retrieves the number units that the player has killed.</summary>
    ///
    /// <param name="unit"> (optional)
    ///   The unit type to query. UnitType macros are accepted. If this parameter is omitted,
    ///   then it will use UnitTypes::AllUnits by default.
    /// </param>
    ///
    /// @returns The total number of units that the player has killed throughout the game.
    int killedUnitCount(UnitType unit = UnitTypes::AllUnits) const {
      return unit ? getPlayerData().killedUnitCount[static_cast<int>(unit)] : 0;
    }

    /// <summary>Retrieves the current upgrade level that the player has attained for a given
    /// upgrade type.</summary>
    ///
    /// <param name="upgrade">
    ///   The UpgradeType to query.
    /// </param>
    ///
    /// @returns The number of levels that the \p upgrade has been upgraded for this player.
    ///
    /// Example usage:
    /// @code
    ///   bool obtainNextUpgrade(BWAPI::UpgradeType upgType)
    ///   {
    ///     BWAPI::Player self = BWAPI::Broodwar->self();
    ///     int maxLvl      = self->getMaxUpgradeLevel(upgType);
    ///     int currentLvl  = self->getUpgradeLevel(upgType);
    ///     if ( !self->isUpgrading(upgType) && currentLvl < maxLvl &&
    ///           self->completedUnitCount(upgType.whatsRequired(currentLvl+1)) > 0 &&
    ///           self->completedUnitCount(upgType.whatUpgrades()) > 0 )
    ///       return self->getUnits().upgrade(upgType);
    ///     return false;
    ///   }
    /// @endcode
    ///
    /// @see UnitInterface::upgrade, getMaxUpgradeLevel
    int getUpgradeLevel(UpgradeType upgrade) const {
      return upgrade ? getPlayerData().upgradeLevel[static_cast<int>(upgrade)] : 0;
    }

    /// <summary>Checks if the player has already researched a given technology.</summary>
    ///
    /// <param name="tech">
    ///   The TechType to query.
    /// </param>
    ///
    /// @returns true if the player has obtained the given \p tech, or false if they have not
    /// @see isResearching, UnitInterface::research, isResearchAvailable
    bool hasResearched(TechType tech) const {
      return tech ? getPlayerData().hasResearched[static_cast<int>(tech)] : false;
    }

    /// <summary>Checks if the player is researching a given technology type.</summary>
    ///
    /// <param name="tech">
    ///   The TechType to query.
    /// </param>
    ///
    /// @returns true if the player is currently researching the \p tech, or false otherwise
    /// @see UnitInterface::research, hasResearched
    bool isResearching(TechType tech) const {
      return tech ? getPlayerData().isResearching[static_cast<int>(tech)] : false;
    }

    /// <summary>Checks if the player is upgrading a given upgrade type.</summary>
    ///
    /// <param name="upgrade">
    ///   The upgrade type to query.
    /// </param>
    ///
    /// @returns true if the player is currently upgrading the given \p upgrade, false otherwise
    ///
    /// Example usage:
    /// @code
    ///   bool obtainNextUpgrade(BWAPI::UpgradeType upgType)
    ///   {
    ///     BWAPI::Player self = BWAPI::Broodwar->self();
    ///     int maxLvl      = self->getMaxUpgradeLevel(upgType);
    ///     int currentLvl  = self->getUpgradeLevel(upgType);
    ///     if ( !self->isUpgrading(upgType) && currentLvl < maxLvl &&
    ///           self->completedUnitCount(upgType.whatsRequired(currentLvl+1)) > 0 &&
    ///           self->completedUnitCount(upgType.whatUpgrades()) > 0 )
    ///       return self->getUnits().upgrade(upgType);
    ///     return false;
    ///   }
    /// @endcode
    ///
    /// @see UnitInterface::upgrade
    bool isUpgrading(UpgradeType upgrade) const {
      return upgrade ? getPlayerData().isUpgrading[static_cast<int>(upgrade)] : false;
    }

    /// <summary>Retrieves the color value of the current player.</summary>
    ///
    /// @returns Color object that represents the color of the current player.
    BWAPI::Color getColor() const { return getPlayerData().color; }

    /// <summary>Retrieves the control code character that changes the color of text messages to
    /// represent this player.</summary>
    ///
    /// @returns character code to use for text in Broodwar.
    char getTextColor() const;

    /// <summary>Retrieves the maximum amount of energy that a unit type will have, taking the
    /// player's energy upgrades into consideration.</summary>
    ///
    /// <param name="unit">
    ///   The UnitType to retrieve the maximum energy for.
    /// </param>
    ///
    /// @returns Maximum amount of energy that the given unit type can have.
    int maxEnergy(UnitType unit) const;

    /// <summary>Retrieves the top speed of a unit type, taking the player's speed upgrades into
    /// consideration.</summary>
    ///
    /// <param name="unit">
    ///   The UnitType to retrieve the top speed for.
    /// </param>
    ///
    /// @returns Top speed of the provided unit type for this player.
    double topSpeed(UnitType unit) const;

    /// <summary>Retrieves the maximum weapon range of a weapon type, taking the player's weapon
    /// upgrades into consideration.</summary>
    ///
    /// <param name="weapon">
    ///   The WeaponType to retrieve the maximum range for.
    /// </param>
    ///
    /// @returns Maximum range of the given weapon type for units owned by this player.
    int weaponMaxRange(WeaponType weapon) const;

    /// <summary>Retrieves the sight range of a unit type, taking the player's sight range
    /// upgrades into consideration.</summary>
    ///
    /// <param name="unit">
    ///   The UnitType to retrieve the sight range for.
    /// </param>
    ///
    /// @returns Sight range of the provided unit type for this player.
    int sightRange(UnitType unit) const;

    /// <summary>Retrieves the weapon cooldown of a unit type, taking the player's attack speed
    /// upgrades into consideration.</summary>
    ///
    /// <param name="unit">
    ///   The UnitType to retrieve the damage cooldown for.
    /// </param>
    ///
    /// @returns Weapon cooldown of the provided unit type for this player.
    int weaponDamageCooldown(UnitType unit) const;

    /// <summary>Calculates the armor that a given unit type will have, including upgrades.</summary>
    ///
    /// <param name="unit">
    ///   The unit type to calculate armor for, using the current player's upgrades.
    /// </param>
    ///
    /// @returns The amount of armor that the unit will have with the player's upgrades.
    int armor(UnitType unit) const;

    /// <summary>Calculates the damage that a given weapon type can deal, including upgrades.</summary>
    ///
    /// <param name="wpn">
    ///   The weapon type to calculate for.
    /// </param>
    ///
    /// @returns The amount of damage that the weapon deals with this player's upgrades.
    int damage(WeaponType wpn) const;

    /// <summary>Retrieves the total unit score, as seen in the end-game score screen.</summary>
    ///
    /// @returns The player's unit score.
    int getUnitScore() const { return getPlayerData().totalUnitScore; }

    /// <summary>Retrieves the total kill score, as seen in the end-game score screen.</summary>
    ///
    /// @returns The player's kill score.
    int getKillScore() const { return getPlayerData().totalKillScore; }

    /// <summary>Retrieves the total building score, as seen in the end-game score screen.</summary>
    ///
    /// @returns The player's building score.
    int getBuildingScore() const { return getPlayerData().totalBuildingScore; }

    /// <summary>Retrieves the total razing score, as seen in the end-game score screen.</summary>
    ///
    /// @returns The player's razing score.
    int getRazingScore() const { return getPlayerData().totalRazingScore; }

    /// <summary>Retrieves the player's custom score.</summary> This score is used in @UMS game
    /// types.
    ///
    /// @returns The player's custom score.
    int getCustomScore() const { return getPlayerData().customScore; }

    /// <summary>Checks if the player is an observer player, typically in a @UMS observer
    /// game.</summary> An observer player does not participate in the game.
    ///
    /// @returns true if the player is observing, or false if the player is capable of playing in
    /// the game.
    bool isObserver() const { return !getPlayerData().isParticipating; }

    /// <summary>Retrieves the maximum upgrades available specific to the player.</summary> This
    /// value is only different from UpgradeType::maxRepeats in @UMS games.
    ///
    /// <param name="upgrade">
    ///   The UpgradeType to retrieve the maximum upgrade level for.
    /// </param>
    ///
    /// @returns Maximum upgrade level of the given \p upgrade type.
    ///
    /// Example usage:
    /// @code
    ///   bool obtainNextUpgrade(BWAPI::UpgradeType upgType)
    ///   {
    ///     BWAPI::Player self = BWAPI::Broodwar->self();
    ///     int maxLvl      = self->getMaxUpgradeLevel(upgType);
    ///     int currentLvl  = self->getUpgradeLevel(upgType);
    ///     if ( !self->isUpgrading(upgType) && currentLvl < maxLvl &&
    ///           self->completedUnitCount(upgType.whatsRequired(currentLvl+1)) > 0 &&
    ///           self->completedUnitCount(upgType.whatUpgrades()) > 0 )
    ///       return self->getUnits().upgrade(upgType);
    ///     return false;
    ///   }
    /// @endcode
    int getMaxUpgradeLevel(UpgradeType upgrade) const {
      return upgrade ? getPlayerData().maxUpgradeLevel[static_cast<int>(upgrade)] : 0;
    }

    /// <summary>Checks if a technology can be researched by the player.</summary> Certain
    /// technologies may be disabled in @UMS game types.
    ///
    /// <param name="tech">
    ///   The TechType to query.
    /// </param>
    ///
    /// @returns true if the \p tech type is available to the player for research.
    bool isResearchAvailable(TechType tech) const {
      return tech ? getPlayerData().isResearchAvailable[static_cast<int>(tech)] : false;
    }

    /// <summary>Checks if a unit type can be created by the player.</summary> Certain unit types
    /// may be disabled in @UMS game types.
    ///
    /// <param name="unit">
    ///   The UnitType to check.
    /// </param>
    ///
    /// @returns true if the \p unit type is available to the player.
    bool isUnitAvailable(UnitType unit) const {
      return unit ? getPlayerData().isUnitAvailable[static_cast<int>(unit)] : false;
    }

    /// <summary>Verifies that this player satisfies a unit type requirement.</summary>
    /// This verifies complex type requirements involving morphable @Zerg structures. For example,
    /// if something requires a @Spire, but the player has (or is in the process of morphing) a
    /// @Greater_Spire, this function will identify the requirement. It is simply a convenience function
    /// that performs all of the requirement checks.
    ///
    /// <param name="unit">
    ///   The UnitType to check.
    /// </param>
    /// <param name="amount"> (optional)
    ///   The amount of units that are required.
    /// </param>
    ///
    /// @returns true if the unit type requirements are met, and false otherwise.
    ///
    /// @since 4.1.2
    bool hasUnitTypeRequirement(UnitType unit, int amount = 1) const;

    constexpr Player *operator->() { return this; }
    constexpr Player const *operator->() const { return this; }
  };
};
