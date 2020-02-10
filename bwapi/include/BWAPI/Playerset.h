#pragma once
#include <BWAPI/IDs.h>
#include <BWAPI/SetContainer.h>
#include <BWAPI/Race.h>
#include <BWAPI/Player.h>

namespace BWAPI
{
  // Forward Declarations
  class Unitset;

  /// <summary>A set containing Player objects.</summary>
  class Playerset : public SetContainer<BWAPI::Player, IDCompare>
  {
  public:

    /// <summary>Returns the set of all units that every player in this set owns.</summary>
    ///
    /// @returns Unitset containing Playerset's units
    /// @see Player::getUnits
    Unitset getUnits() const;

    /// <summary>Returns the list of races that each player in the set is.</summary>
    ///
    /// @returns Race::set containing Playerset's races
    /// @see Player::getRace
    Race::set getRaces() const;

    /// <summary>Sets the alliance status with all players contained in the Playerset.</summary>
    ///
    /// <param name="allies">
    ///     Set to true to set the player to allied, or false for enemy.
    /// </param>
    /// <param name="alliedVictory">
    ///     Set to true to turn on allied victory, or false to disable it.
    /// </param>
    ///
    /// @see Game::setAlliance
    void setAlliance(bool allies = true, bool alliedVictory = true);

  };
}

