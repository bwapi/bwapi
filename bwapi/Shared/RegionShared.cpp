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
  bool RegionImpl::isAccessible() const
  {
    return self->isAccessible;
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
  const Regionset &RegionImpl::getNeighbors() const
  {
    return this->neighbors;
  }
  BWAPI::Region RegionImpl::getClosestAccessibleRegion() const
  {
    return this->closestAccessibleRgn;
  }
  BWAPI::Region RegionImpl::getClosestInaccessibleRegion() const
  {
    return this->closestInaccessibleRgn;
  }
}
