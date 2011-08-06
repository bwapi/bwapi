#include "Client.h"
#include "RegionImpl.h"

namespace BWAPI
{
  RegionImpl::RegionImpl(int index)
  {
    this->self = &BWAPI::BWAPIClient.data->regions[index];
  }
  void RegionImpl::setNeighbors()
  {
    int accessibleBestDist    = 99999;
    int inaccessibleBestDist  = 99999;
    for ( int i = 0; i < self->neighborCount; ++i )
    {
      BWAPI::Region *neighbor = Broodwar->getRegion(self->neighbors[i]);
      this->neighbors.insert(neighbor);

      // Obtain the closest accessable and inaccessable Regions from their Region center
      int dst = this->getCenter().getApproxDistance(neighbor->getCenter());
      if ( this->getRegionGroupID() == neighbor->getRegionGroupID() )
      {
        if ( dst < accessibleBestDist )
        {
          accessibleBestDist = dst;
          this->closestAccessibleRgn = neighbor;
        }
      }
      else if ( dst < inaccessibleBestDist )
      {
        inaccessibleBestDist = dst;
        this->closestInaccessibleRgn = neighbor;
      }
    }
  }
  int RegionImpl::getRegionGroupID() const
  {
    return self->islandID;
  }
  BWAPI::Position RegionImpl::getCenter() const
  {
    return BWAPI::Position(self->center_x, self->center_y);
  }
  bool RegionImpl::isHigherGround() const
  {
    return self->isHigherGround;
  }
  int RegionImpl::getDefensePriority() const
  {
    return self->priority;
  }
  bool RegionImpl::isWalkable() const
  {
    return self->isWalkable;
  }
  int RegionImpl::getID() const
  {
    return self->id;
  }
  int RegionImpl::getBoundsLeft() const
  {
    return self->leftMost;
  }
  int RegionImpl::getBoundsTop() const
  {
    return self->topMost;
  }
  int RegionImpl::getBoundsRight() const
  {
    return self->rightMost;
  }
  int RegionImpl::getBoundsBottom() const
  {
    return self->bottomMost;
  }
  const std::set<BWAPI::Region*> &RegionImpl::getNeighbors() const
  {
    return this->neighbors;
  }
  BWAPI::Region *RegionImpl::getClosestAccessibleRegion() const
  {
    return this->closestAccessibleRgn;
  }
  BWAPI::Region *RegionImpl::getClosestInaccessibleRegion() const
  {
    return this->closestInaccessibleRgn;
  }
  int RegionImpl::getDistance(BWAPI::Region *other) const
  {
    return this->getCenter().getApproxDistance(other->getCenter());
  }
};
