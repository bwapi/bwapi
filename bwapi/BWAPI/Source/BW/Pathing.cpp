#include <set>

#include "util/foreach.h"

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

  u16 getPath(region *target, region *previous, region *current, u16 iteration, Position *out_points, u16 outSize, bool *eliminated)
  {
    if ( current == target )
    {
      out_points[iteration] = current->getCenter();
      return iteration;
    }
    
    if ( iteration >= outSize )
      return 0;

    for ( u8 i = 0; i < current->neighborCount; ++i )
    {
      u16 nId = current->neighbors[i];
      if ( eliminated[nId] )
        continue;

      region *neighbor = getRegion(nId);
      if ( neighbor == previous || !current->isConnectedTo( neighbor ) )
        continue;

      u16 lIter = getPath(target, current, neighbor, iteration + 1, out_points, outSize, eliminated);
      if ( lIter == 0 )
        continue;

      out_points[iteration] = neighbor->getCenter();
      return lIter;
    }
    eliminated[current->getIndex()] = true;
    return 0;
  }

  u16 region::getPointPath(region *target, Position *out_points, u16 outSize)
  {
    //std::list<Position> result;
    //memset(out_points, 0, sizeof(Position) * outSize);
    u16 rval = 0;
    bool eliminated[5000];
    memset(eliminated, 0, sizeof(bool) * 5000);
    if ( this != target )
      rval = getPath(target, NULL, this, 0, out_points, outSize, eliminated);
    return rval;
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

  u16 region::getIndex()
  {
    return (u16)( ((u32)this - (u32)(&BW::BWDATA_SAIPathing->regions)) / sizeof(BW::region) );
  }
};

