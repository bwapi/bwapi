#include "RegionImpl.h"

namespace BWAPI
{
  // -------------------------------- GET GROUP ID -----------------------------------------------------------
  int RegionImpl::getRegionGroupID() const
  {
    return self->islandID;
  }
  // --------------------------------- GET CENTER ------------------------------------------------------------
  BWAPI::Position RegionImpl::getCenter() const
  {
    return BWAPI::Position(self->center_x, self->center_y);
  }
  // --------------------------------- HIGHER GRND -----------------------------------------------------------
  bool RegionImpl::isHigherGround() const
  {
    return self->isHigherGround;
  }
  // ------------------------------ DEFENSE PRIORITY ---------------------------------------------------------
  int RegionImpl::getDefensePriority() const
  {
    return self->priority;
  }
  // ----------------------------------- WALKABLE ------------------------------------------------------------
  bool RegionImpl::isWalkable() const
  {
    return self->isWalkable;
  }
  // ------------------------------------ GET ID -------------------------------------------------------------
  int RegionImpl::getID() const
  {
    return self->id;
  }
  // ------------------------------------ BOUNDS -------------------------------------------------------------
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
  // ---------------------------------- NEIGHBOURS -----------------------------------------------------------
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
}
