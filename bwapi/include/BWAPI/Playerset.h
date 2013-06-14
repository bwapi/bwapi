#pragma once
#include <BWAPI/Vectorset.h>

#include <BWAPI/Race.h>
#include <BWAPI/Filters.h>

namespace BWAPI
{
  // Forward Declarations
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class Unitset;

  class Playerset : public Vectorset<BWAPI::Player>
  {
  public:
    Playerset(size_t initialSize = 16);
    Playerset(const Playerset &other);
    Playerset(Playerset &&other);

    /// Returns the set of all units that every
    /// player in this set owns.
    ///
    /// @returns Unitset containing Playerset's units
    /// @see PlayerInterface::getUnits
    Unitset getUnits() const;

    /// Returns the list of races that each player
    /// in the set is.
    ///
    /// @returns Race::set containing Playerset's races
    /// @see PlayerInterface::getRace
    Race::set getRaces() const;

    /// Sets the alliance status with all players
    /// contained in the Playerset.
    ///
    /// @param allies Set to true to set the player
    /// to allied, or false for enemy.
    /// @param alliedVictory Set to true to turn on
    /// allied victory, or false to disable it
    /// @see Game::setAlliance
    void setAlliance(bool allies = true, bool alliedVictory = true);

  };
}

