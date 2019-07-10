#pragma once
#include <string_view>
#include <BWAPI/IDs.h>
#include <BWAPI/Client/ForceData.h>

namespace BWAPI
{
  class Playerset;

  /// <summary>The Force class is used to get information about each force in a match.</summary>
  /// Normally this is considered a team.
  ///
  /// @note It is not called a team because players on the same force do not necessarily need
  /// to be allied at the beginning of a match.
  ///
  /// @ingroup Interface
  class Force : public InterfaceDataWrapper<Force, ForceData>
  {
  public:
    using InterfaceDataWrapper<Force, ForceData>::InterfaceDataWrapper;
    /// <summary>Retrieves the name of the Force.</summary>
    ///
    /// @returns
    ///   A std::string object containing the name of the force.
    ///
    /// Example usage:
    /// @code
    ///   BWAPI::Force myForce = BWAPI::Broodwar->self()->getForce();
    ///   if ( myForce->getName() == "Observers" )
    ///     BWAPI::Broodwar << "Looks like we're observing a match." << std::endl;
    /// @endcode
    ///
    /// @note Don't forget to use std::string::c_str() when passing this parameter to
    /// Game::sendText and other variadic functions.
    std::string_view getName() const { return getData().name; }

    /// <summary>Retrieves the set of players that belong to this Force.</summary>
    ///
    /// @returns
    ///   A Playerset object containing the players that are part of this Force.
    ///
    /// Example usage:
    /// @code
    ///   // Get the enemy force, but make sure we have an enemy
    ///   BWAPI::Force myEnemyForce = BWAPI::Broodwar->enemy() ? BWAPI::Broodwar->enemy()->getForce() : nullptr;
    ///   if ( myEnemyForce != nullptr )
    ///   {
    ///     Broodwar << "The allies of my enemy are..." << std::endl;
    ///     for ( auto i = myEnemyForce.begin(); i != myEnemyForce.end(); ++i )
    ///       Broodwar << "  - " << i->getName() << std::endl;
    ///   }
    /// @endcode
    Playerset getPlayers() const;
  };
}
