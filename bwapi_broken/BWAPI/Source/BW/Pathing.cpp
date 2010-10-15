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

  u16 region::getDistance(region *dst)
  {
    return dst->getCenter().getDistance(this->getCenter());
  }

  bool getPath(region *target, region *current, u16 iteration, u16 *out_regions, u16 *best_regions, u32 *best_length, u32 *this_length, u16 *bestSize, u16 outSize, bool *eliminated)
  {
    if ( current == target )
    {
      out_regions[iteration] = current->getIndex();
      if ( *this_length < *best_length )
      {
        *best_length = *this_length;
        *bestSize = iteration + 1;
        memcpy(best_regions, out_regions, iteration + 1);
      }
      return true;
    }
    
    if ( iteration >= outSize || iteration > 256 )
      return false;

    for ( u32 n = 0; n < current->neighborCount; ++n )
    {
      u16 nId = current->neighbors[n];
      if ( eliminated[nId] )
        continue;

      region *neighbor = getRegion(nId);
      if ( !current->isConnectedTo( neighbor ) )
        continue;
      
      out_regions[iteration] = nId;
      u16 curLen             = current->getDistance(neighbor);

      eliminated[nId] = true;
      *this_length    += curLen;
      getPath(target, neighbor, iteration + 1, out_regions, best_regions, best_length, this_length, bestSize, outSize, eliminated);
      *this_length    -= curLen;
      eliminated[nId] = false;
    }
    if ( iteration == 1 ) // not returning to iter 1
      MessageBox(NULL, "p", "!", MB_OK);
    return *bestSize != 0;
  }

  u16 region::getPointPath(region *target, Position *out_points, u16 outSize)
  {
    //std::list<Position> result;
    //memset(out_points, 0, sizeof(Position) * outSize);
    bool eliminated[5000];
    memset(eliminated, 0, sizeof(bool) * 5000);

    u16 out_regions[5000];
    memset(out_regions, 0, sizeof(u16) * 5000);
    u16 best_Regions[5000];
    memset(best_Regions, 0, sizeof(u16) * 5000);
    u32 best_length = 0x7FFFFFFF;
    u32 this_length = 0;
    u16 best_size   = 0;

    eliminated[0] = true;
    eliminated[this->getIndex()] = true;

    if ( this != target )
      getPath(target, this, 0, out_regions, best_Regions, &best_length, &this_length, &best_size, outSize > 5000 ? 5000 : outSize, eliminated);

    for ( int i = 0; i < best_size && i < outSize; ++i )
      out_points[i] = getRegion(best_Regions[i])->getCenter();
    return best_size;
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

