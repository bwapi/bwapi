#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/Filters.h>
#include <BWAPI/UnaryFilter.h>

namespace BWAPI
{
  class Regionset;
  class Unitset;

  class RegionInterface;
  typedef RegionInterface *Region;

  /// <summary>Region objects are created by Starcraft: Broodwar to contain several tiles with the same
  /// properties, and create a node in pathfinding and other algorithms.</summary> Regions may not contain
  /// detailed information, but have a sufficient amount of data to identify general chokepoints,
  /// accessibility to neighboring terrain, be used in general pathing algorithms, and used as
  /// nodes to rally units to.
  ///
  /// Most parameters that are available are explicitly assigned by Broodwar itself.
  ///
  /// @see Game::getAllRegions, Game::getRegionAt, UnitInterface::getRegion
  /// @ingroup Interface
  class RegionInterface
  {
  protected:
    virtual ~RegionInterface() {};
  public:
    virtual int getID() const = 0;

    virtual int getRegionGroupID() const = 0;

    virtual BWAPI::Position getCenter() const = 0;

    virtual bool isHigherGround() const = 0;

    virtual int getDefensePriority() const = 0;

    virtual bool isAccessible() const = 0;

    virtual const Regionset &getNeighbors() const = 0;

    virtual int getBoundsLeft() const = 0;

    virtual int getBoundsTop() const = 0;

    virtual int getBoundsRight() const = 0;

    virtual int getBoundsBottom() const = 0;

    virtual BWAPI::Region getClosestAccessibleRegion() const = 0;

    virtual BWAPI::Region getClosestInaccessibleRegion() const = 0;

    int getDistance(BWAPI::Region other) const;

    Unitset getUnits(const UnitFilter &pred = nullptr) const;
  };
};
