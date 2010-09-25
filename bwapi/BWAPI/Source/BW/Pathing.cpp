#include "Pathing.h"
#include "Offsets.h"

namespace BW
{
  bool region::isConnectedTo(region *target)
  {
    if ( target != NULL )
      return this->groupIndex == target->groupIndex;
    return false;
  }

  bool region::isConnectedTo(u16 index)
  {
    if ( index < BW::BWDATA_SAIPathing->regionCount )
      return this->groupIndex == getRegion(index)->groupIndex;
    return false;
  }

  u8 region::getAccessibleNeighbours(region **out_regions, u8 outputCount)
  {
    u8 count = 0;
    for ( u8 i = 0; i < this->neighborCount && count < outputCount; ++i )
    {
      region *neighbor = this->getNeighbor(i);
      if ( this->isConnectedTo( neighbor ) )
      {
        out_regions[i] = neighbor;
        ++count;
      }
    }
    return count;
  }

  region *region::getNeighbor(u8 index)
  {
    if ( index <= this->neighborCount )
      return getRegion(this->neighbors[index]);
    return NULL;
  }

  Position region::getCenter()
  {
    if ( this != NULL )
      return BW::Position((u16)(this->rgnCenterX >> 8), (u16)(this->rgnCenterY >> 8));
    return BW::Position(0,0);
  }
};

