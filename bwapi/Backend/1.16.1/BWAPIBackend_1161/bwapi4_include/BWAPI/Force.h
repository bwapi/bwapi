#pragma once
#include <string>
#include <BWAPI/Interface.h>

namespace BWAPI
{
  // Forward Declarations
  class Playerset;

  class ForceInterface;
  typedef ForceInterface *Force;

  /// <summary>The Force class is used to get information about each force in a match.</summary>
  /// Normally this is considered a team.
  ///
  /// @note It is not called a team because players on the same force do not necessarily need
  /// to be allied at the beginning of a match.
  ///
  /// @ingroup Interface
  class ForceInterface : public Interface<ForceInterface>
  {
  protected:
    virtual ~ForceInterface() {};
  public :
    /// <summary>Retrieves the unique ID that represents this Force.</summary>
    ///
    /// @returns
    ///   An integer containing the ID for the Force.
    virtual int getID() const = 0;

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
    virtual std::string getName() const = 0;

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
    virtual Playerset getPlayers() const = 0;
  };
}
