#pragma once
#include <set>
#include <string>

namespace BWAPI
{
  class Player;

  /** The Force class is used to get information about each force in the match, such as the name of the force
   * and the set of players in the force. */
  class Force
  {
    public :
      /** Returns a unique ID for the force. */
      virtual int getID() const = 0;

      /** Returns the name of the force. */
      virtual std::string getName() const = 0;

      /** Returns the set of players in the force. */
      virtual std::set<Player*> getPlayers() const = 0;
  };
}
