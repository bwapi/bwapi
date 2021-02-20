#include <BWAPI4/Unitset.h>
#include <BWAPI/Position.h>
#include "../Source/BWAPI/UnitImpl.h"

namespace BWAPI4
{
  // initialize no-set
  const Unitset Unitset::none;

  ////////////////////////////////////////////////////////// Position
  BWAPI::Position Unitset::getPosition() const
  {
    // Declare the local position
    BWAPI::Position retPosition(0,0);
    int validPosCount = 0;

    // Add up the positions for all units in the set
    for ( auto &u : *this )
    {
      BWAPI::Position pos = u->getPosition();
      if ( pos.x >= 0 && pos.y >= 0 )
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

