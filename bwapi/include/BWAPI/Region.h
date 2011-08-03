#pragma once

#include <set>
#include <BWAPI/Position.h>

namespace BWAPI
{
  class Region
  {
  public:
    /** Returns the unique ID for this region assigned by Broodwar. */
    virtual int getID() const = 0;

    /** Returns an ID for the body of land that this region belongs to. */
    virtual int getRegionGroupID() const = 0;

    /** Retrieves the center of the region. */
    virtual BWAPI::Position getCenter() const = 0;

    /** Returns true if this region belongs to strategic higher ground, assigned by Broodwar. */
    virtual bool isHigherGround() const = 0;

    /** Returns a value representing the strategic advantage of this region according to Broodwar. */
    virtual int getDefensePriority() const = 0;

    /** Returns true if the region can be walked on by ground units. */
    virtual bool isWalkable() const = 0;

    /** Retrieves the set of neighbors that this region is connected to. */
    virtual const std::set<Region*> &getNeighbors() const = 0;

    /** Retrieves the approximate region boundry as specified by Broodwar. */
    virtual int getBoundsLeft() const = 0;
    virtual int getBoundsTop() const = 0;
    virtual int getBoundsRight() const = 0;
    virtual int getBoundsBottom() const = 0;
  };
};
