#include <set>
#include <list>
#include <bitset>
#include <vector>

#include "util/foreach.h"

#include "Pathing.h"
#include "Offsets.h"

#include "../../Debug.h"

namespace BW
{
  bool region::isConnectedTo(region *target)
  {
    if ( target )
      return this->groupIndex == target->groupIndex;
    return false;
  }

  bool region::isConnectedTo(u16 index)
  {
    if ( index < BW::BWDATA_SAIPathing->regionCount )
      return this->groupIndex == getRegionFromId(index)->groupIndex;
    return false;
  }

  std::vector<region*> &region::getAccessibleNeighbours()
  {
    static std::vector<region*> neighbors;
    neighbors.clear();
    for ( u8 i = 0; i < this->neighborCount; ++i )
    {
      region *neighbor = this->getNeighbor(i);
      if ( this->isConnectedTo( neighbor ) )
        neighbors.push_back(neighbor);
    }
    return neighbors;
  }

  int region::getAirDistance(region *dst)
  {
    return dst->getCenter().getApproxDistance(this->getCenter());
  }

  #define MAX_STEPS 100
  bool calcRoughPath(region *current, region *target, std::vector<region*> *regionList, int step, int currentDistance, int *bestDistance, std::bitset<5000> *tested, std::vector<region*> *bestRegionList)
  {
    // If we've reached our goal
    if ( current == target )
    {
      if ( currentDistance > *bestDistance )
        return false;
      *bestDistance = currentDistance;
      (*bestRegionList) = (*regionList);
      return true;
    }

    // don't exceed max steps
    if ( step >= MAX_STEPS )
      return false;

    // iterate all accessable neighbors
    (*regionList).push_back(current);
    (*tested).set(current->getIndex(), true);
    std::vector<region*> accessibleNeighborsCopy = current->getAccessibleNeighbours();
    foreach( region *r, accessibleNeighborsCopy )
    {
      // Skip this entry if we've already passed through it
      if ( (*tested).test(r->getIndex()) )
        continue;
      
      // Obtain the total distance and skip this entry if it's larger than the best distance
      int nextDistance = currentDistance + current->getAirDistance(r);
      if ( nextDistance > *bestDistance )
        continue;
      
      // Perform nested call on neighbor
      calcRoughPath(r, target, regionList, step + 1, nextDistance, bestDistance, tested, bestRegionList);
    }
    (*tested).set(current->getIndex(), false);
    (*regionList).pop_back();
    return false;
  }

  std::vector<region*> &region::getRoughPath(region *target)
  {
    static std::vector<region*> regions;
    regions.clear();

    if ( this->groupIndex != target->groupIndex )
      return regions;
    std::bitset<5000> tested;
    tested.reset();

    int best = MAXINT;
    std::vector<region*> tempRgns;
    calcRoughPath(this, target, &tempRgns, 0, 0, &best, &tested, &regions);
    return regions;
  }
  region *region::getNeighbor(u8 index)
  {
    if ( index <= this->neighborCount )
      return getRegionFromId(this->neighbors[index]);
    return NULL;
  }

  Position region::getCenter()
  {
    return BW::Position((u16)(this->rgnCenterX >> 8), (u16)(this->rgnCenterY >> 8));
  }

  u16 region::getIndex()
  {
    return (u16)( ((u32)this - (u32)(&BW::BWDATA_SAIPathing->regions)) / sizeof(BW::region) );
  }
};

