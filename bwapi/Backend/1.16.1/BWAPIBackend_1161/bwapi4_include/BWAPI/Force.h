#pragma once
#include <string>

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
  class ForceInterface
  {
  protected:
    virtual ~ForceInterface() {};
  public :
    virtual int getID() const = 0;

    virtual std::string getName() const = 0;

    virtual Playerset getPlayers() const = 0;
  };
}
