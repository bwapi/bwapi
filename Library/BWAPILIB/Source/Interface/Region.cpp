#include <BWAPI/Region.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/Game.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Filters.h>

namespace BWAPI
{
  int Region::getRegionGroupID() const
  {
    return dataptr->islandID;
  }

  Position Region::getCenter() const
  {
    return Position{ dataptr->center_x, dataptr->center_y };
  }

  bool Region::isHigherGround() const
  {
    return dataptr->isHigherGround;
  }

  int Region::getDefensePriority() const
  {
    return dataptr->priority;
  }

  bool Region::isAccessible() const
  {
    return dataptr->isAccessible;
  }

  const Regionset Region::getNeighbors() const
  {
    Regionset retSet;
    for (auto r : dataptr->neighbors)
    {
      retSet.emplace(getGame().getRegion(r));
    }
    return retSet;
  }

  int Region::getBoundsLeft() const
  {
    return dataptr->leftMost;
  }

  int Region::getBoundsTop() const
  {
    return dataptr->topMost;
  }

  int Region::getBoundsRight() const
  {
    return dataptr->rightMost;
  }

  int Region::getBoundsBottom() const
  {
    return dataptr->bottomMost;
  }

  Region Region::getClosestAccessibleRegion() const
  {
    // TODO: Make this a precalced field that's stored in RegionData
    int accessibleBestDist = 99999;
    Region ret = nullptr;
    for (auto r : dataptr->neighbors)
    {
      auto dist = this->getCenter().getApproxDistance(getGame().getRegion(r).getCenter());
      if (dist < accessibleBestDist)
      {
        accessibleBestDist = dist;
        ret = getGame().getRegion(r);
      }
    }
    return ret;
  }

  Region Region::getClosestInaccessibleRegion() const
  {
    int inaccessibleDist = 99999;
    Region ret = nullptr;
    for (auto r : getGame().getAllRegions())
    {
      if (r.getData().islandID != dataptr->islandID)
      {
        auto dist = r.getCenter().getApproxDistance(this->getCenter());
        if (dist < inaccessibleDist)
        {
          inaccessibleDist = dist;
          ret = r;
        }
      }
    }
    return ret;
  }

  int Region::getDistance(BWAPI::Region other) const
  {
    return this->getCenter().getApproxDistance(other->getCenter());
  }

  Unitset Region::getUnits(const UnitFilter &pred) const
  {
    return getGame().getUnitsInRectangle(this->getBoundsLeft(),
                                         this->getBoundsTop(),
                                         this->getBoundsRight(),
                                         this->getBoundsBottom(),
                                         [&](Unit u){ return u->getRegion() == *this && (!pred.isValid() || pred(u)); });
  }
}
