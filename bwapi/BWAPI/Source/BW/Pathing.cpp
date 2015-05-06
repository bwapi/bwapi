#include <vector>
#include <sstream>

#include "Pathing.h"
#include "Offsets.h"

#include "../../../Debug.h"

namespace BW
{
  bool region::isConnectedTo(region *target) const
  {
    if ( target )
      return this->groupIndex == target->groupIndex;
    return false;
  }

  bool region::isConnectedTo(u16 index) const
  {
    if ( index < BW::BWDATA_SAIPathing->regionCount )
      return this->groupIndex == getRegionFromId(index)->groupIndex;
    return false;
  }

  std::vector<region*> &region::getAccessibleNeighbours() const
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

  int region::getAirDistance(region *dst) const
  {
    return dst->getCenter().getApproxDistance(this->getCenter());
  }

  region *region::getNeighbor(u8 index) const
  {
    if ( index <= this->neighborCount )
      return getRegionFromId(this->neighbors[index]);
    return NULL;
  }

  Position region::getCenter() const
  {
    return BW::Position((u16)(this->rgnCenterX >> 8), (u16)(this->rgnCenterY >> 8));
  }

  u16 region::getIndex() const
  {
    return (u16)( ((u32)this - (u32)(BW::BWDATA_SAIPathing->regions)) / sizeof(BW::region) );
  }

  BW::region *getRegionAt(int x, int y)
  {
    int tileX = x / 32, tileY = y / 32;
    if ( tileX < 0 || tileY < 0 || tileX >= 256 || tileY >= 256 )
      return NULL;

    // Obtain the region IDs from the positions
    u16 id = BW::BWDATA_SAIPathing->mapTileRegionId[tileY][tileX];

    // Check if the id is splitting the tile between regions
    if ( id & 0xE000 )
    {
      // Correct the ID (convert higher bits)
      id = (id & 0x1FFF) + ((id & 0xC000) >> 1);

      // Get source region from split-tile based on walk tile
      int minitilePosX = (x&0x1F)/8;
      int minitilePosY = (y&0x1F)/8;
      int minitileShift = minitilePosX + minitilePosY * 4;
      BW::split *t = &BW::BWDATA_SAIPathing->splitTiles[id];
      if ( (t->minitileMask >> minitileShift) & 1 )
        return &BW::BWDATA_SAIPathing->regions[t->rgn2];
      return &BW::BWDATA_SAIPathing->regions[t->rgn1];
    }
    else if ( id < 5000 )
    {
      // Get source region from tile
      return &BW::BWDATA_SAIPathing->regions[id];
    }
    else
    {
      return NULL;
    }
  }
  BW::region *getRegionAt(Position pos)
  {
    return getRegionAt(pos.x, pos.y);
  }
};

