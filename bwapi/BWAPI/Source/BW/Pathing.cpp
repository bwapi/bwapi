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

