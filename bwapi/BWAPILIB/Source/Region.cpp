#include <BWAPI/Region.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Game.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Filters.h>

namespace BWAPI
{
  int RegionInterface::getDistance(BWAPI::Region other) const
  {
    return this->getCenter().getApproxDistance(other->getCenter());
  }

  Unitset RegionInterface::getUnits(const UnitFilter &pred) const
  {
    return Broodwar->getUnitsInRectangle(this->getBoundsLeft(),
                                         this->getBoundsTop(),
                                         this->getBoundsRight(),
                                         this->getBoundsBottom(),
                                         [&](Unit u){ return u->getRegion() == this && (!pred.isValid() || pred(u)); });
  }
}
