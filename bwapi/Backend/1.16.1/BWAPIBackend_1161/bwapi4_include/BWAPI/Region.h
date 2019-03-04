#pragma once
#include <BWAPI/Position.h>
#include <BWAPI/Filters.h>
#include <BWAPI/UnaryFilter.h>
#include <BWAPI/Interface.h>

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
  class RegionInterface : public Interface<RegionInterface>
  {
  protected:
    virtual ~RegionInterface() {};
  public:
    /// <summary>Retrieves a unique identifier for this region.</summary>
    ///
    /// @note This identifier is explicitly assigned by Broodwar.
    ///
    /// @returns An integer that represents this region.
    /// @see Game::getRegion
    virtual int getID() const = 0;

    /// <summary>Retrieves a unique identifier for a group of regions that are all connected and
    /// accessible by each other.</summary> That is, all accessible regions will have the same
    /// group ID. This function is generally used to check if a path is available between two
    /// points in constant time.
    ///
    /// @note This identifier is explicitly assigned by Broodwar.
    ///
    /// @returns An integer that represents the group of regions that this one is attached to.
    virtual int getRegionGroupID() const = 0;

    /// <summary>Retrieves the center of the region.</summary> This position is used as the node
    /// of the region.
    ///
    /// @returns A Position indicating the center location of the Region, in pixels.
    virtual BWAPI::Position getCenter() const = 0;

    /// <summary>Checks if this region is part of higher ground.</summary> Higher ground may be
    /// used in strategic placement of units and structures.
    ///
    /// @returns true if this region is part of strategic higher ground, and false otherwise.
    virtual bool isHigherGround() const = 0;

    /// <summary>Retrieves a value that represents the strategic advantage of this region relative
    /// to other regions.</summary> A value of 2 may indicate a possible choke point, and a value
    /// of 3 indicates a signficant strategic position.
    ///
    /// @note This value is explicitly assigned by Broodwar.
    ///
    /// @returns An integer indicating this region's strategic potential.
    virtual int getDefensePriority() const = 0;

    /// <summary>Retrieves the state of accessibility of the region.</summary> The region is
    /// considered accessible if it can be accessed by ground units.
    ///
    /// @returns true if ground units can traverse this region, and false if the tiles in this
    /// region are inaccessible or unwalkable.
    virtual bool isAccessible() const = 0;

    /// <summary>Retrieves the set of neighbor Regions that this one is connected to.</summary>
    ///
    /// @returns A reference to a Regionset containing the neighboring Regions.
    virtual const Regionset &getNeighbors() const = 0;

    /// <summary>Retrieves the approximate left boundary of the region.</summary>
    ///
    /// @returns The x coordinate, in pixels, of the approximate left boundary of the region.
    virtual int getBoundsLeft() const = 0;

    /// <summary>Retrieves the approximate top boundary of the region.</summary>
    ///
    /// @returns The y coordinate, in pixels, of the approximate top boundary of the region.
    virtual int getBoundsTop() const = 0;

    /// <summary>Retrieves the approximate right boundary of the region.</summary>
    ///
    /// @returns The x coordinate, in pixels, of the approximate right boundary of the region.
    virtual int getBoundsRight() const = 0;

    /// <summary>Retrieves the approximate bottom boundary of the region.</summary>
    ///
    /// @returns The y coordinate, in pixels, of the approximate bottom boundary of the region.
    virtual int getBoundsBottom() const = 0;

    /// <summary>Retrieves the closest accessible neighbor region.</summary>
    ///
    /// @returns The closest Region that is accessible.
    virtual BWAPI::Region getClosestAccessibleRegion() const = 0;

    /// <summary>Retrieves the closest inaccessible neighbor region.</summary>
    ///
    /// @returns The closest Region that is inaccessible.
    virtual BWAPI::Region getClosestInaccessibleRegion() const = 0;

    /// <summary>Retrieves the center-to-center distance between two regions.</summary>
    ///
    /// @note Ignores all collisions.
    ///
    /// <param name="other">
    ///   The target Region to calculate distance to.
    /// </param>
    /// @returns The integer distance from this Region to \p other.
    int getDistance(BWAPI::Region other) const;

    /// <summary>Retrieves a Unitset containing all the units that are in this region.</summary>
    /// Also has the ability to filter the units before the creation of the Unitset.
    ///
    /// <param name="pred"> (optional)
    ///   If this parameter is used, it is a UnitFilter or function predicate that will retrieve
    ///   only the units whose attributes match the given criteria. If omitted, then a default
    ///   value of nullptr is used, in which case there is no filter.
    /// </param>
    ///
    /// @returns A Unitset containing all units in this region that have met the requirements
    /// of \p pred.
    ///
    /// @see UnitFilter
    Unitset getUnits(const UnitFilter &pred = nullptr) const;
  };
};
