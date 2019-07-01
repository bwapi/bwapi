#include <BWAPI/Region.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Filters.h>

#include "GameImpl.h"

namespace BWAPI
{
  int RegionInterface::getDistance(BWAPI::Region other) const
  {
    return this->getCenter().getApproxDistance(other->getCenter());
  }

  Unitset RegionInterface::getUnits(const UnitFilter &pred) const
  {
    return BroodwarImpl.getUnitsInRectangle(this->getBoundsLeft(),
                                         this->getBoundsTop(),
                                         this->getBoundsRight(),
                                         this->getBoundsBottom(),
                                         [&](Unit u){ return u->getRegion() == this && (!pred.isValid() || pred(u)); });
  }
}
