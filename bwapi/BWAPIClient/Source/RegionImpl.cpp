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
    for ( int i = 0; i < self->neighborCount; ++i )
      this->neighbors.insert(Broodwar->getRegion(self->neighbors[i]));
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
  const std::set<Region*> &RegionImpl::getNeighbors() const
  {
    return this->neighbors;
  }

};
