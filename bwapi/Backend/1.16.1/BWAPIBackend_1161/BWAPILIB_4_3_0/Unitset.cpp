#include <BWAPI4/Unitset.h>
#include <BWAPI4/Position.h>
#include <BWAPI/UnitImpl.h>

namespace BWAPI4
{
  // initialize no-set
  const Unitset Unitset::none;

  ////////////////////////////////////////////////////////// Position
  Position Unitset::getPosition() const
  {
    // Declare the local position
    Position retPosition(0,0);
    int validPosCount = 0;

    // Add up the positions for all units in the set
    for ( auto &u : *this )
    {
      Position pos = u->getPosition();
      if ( pos.isValid() )
      {
        retPosition += pos;
        ++validPosCount;
      }
    }

    if (validPosCount > 0)
      retPosition /= validPosCount;
    return retPosition;
  }
}

