#include <set>

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
      return this->groupIndex == getRegion(index)->groupIndex;
    return false;
  }

  u8 region::getAccessibleNeighbours(region **out_regions, u16 outSize)
  {
    u8 count = 0;
    for ( u8 i = 0; i < this->neighborCount && count < outSize; ++i )
    {
      region *neighbor = this->getNeighbor(i);
      if ( this->isConnectedTo( neighbor ) )
      {
        out_regions[count] = neighbor;
        count++;
      }
    }
    return count;
  }

  u16 region::getDistance(region *dst)
  {
    return dst->getCenter().getDistance(this->getCenter());
  }

  u16 region::getPointPath(region *target, Position *out_points, u16 outSize)
  {
    // @Todo
    return 0;
  }

  region *region::getNeighbor(u8 index)
  {
    if ( index <= this->neighborCount )
      return getRegion(this->neighbors[index]);
    return NULL;
  }

  Position region::getCenter()
  {
    if ( this )
      return BW::Position((u16)(this->rgnCenterX >> 8), (u16)(this->rgnCenterY >> 8));
    return BW::Position(0,0);
  }

  u16 region::getIndex()
  {
    return (u16)( ((u32)this - (u32)(&BW::BWDATA_SAIPathing->regions)) / sizeof(BW::region) );
  }
};

