#include "RegionImpl.h"

namespace BWAPI
{
  // -------------------------------- GET GROUP ID -----------------------------------------------------------
  int RegionImpl::getRegionGroupID() const
  {
    return data.islandID;
  }
  // --------------------------------- GET CENTER ------------------------------------------------------------
  BWAPI::Position RegionImpl::getCenter() const
  {
    return BWAPI::Position(data.center_x, data.center_y);
  }
  // --------------------------------- HIGHER GRND -----------------------------------------------------------
  bool RegionImpl::isHigherGround() const
  {
    return data.isHigherGround;
  }
  // ------------------------------ DEFENSE PRIORITY ---------------------------------------------------------
  int RegionImpl::getDefensePriority() const
  {
    return data.priority;
  }
  // ----------------------------------- WALKABLE ------------------------------------------------------------
  bool RegionImpl::isAccessible() const
  {
    return data.isAccessible;
  }
  // ------------------------------------ GET ID -------------------------------------------------------------
  int RegionImpl::getID() const
  {
    return data.id;
  }
  // ------------------------------------ BOUNDS -------------------------------------------------------------
  int RegionImpl::getBoundsLeft() const
  {
    return data.leftMost;
  }
  int RegionImpl::getBoundsTop() const
  {
    return data.topMost;
  }
  int RegionImpl::getBoundsRight() const
  {
    return data.rightMost;
  }
  int RegionImpl::getBoundsBottom() const
  {
    return data.bottomMost;
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
