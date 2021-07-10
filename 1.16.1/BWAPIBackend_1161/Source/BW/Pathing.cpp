#include <vector>
#include <sstream>

#include "Pathing.h"
#include "Offsets.h"

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
    if ( index < BW::BWDATA::SAIPathing->regionCount )
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
    return BW::Position(static_cast<s16>(this->rgnCenterX >> 8), static_cast<s16>(this->rgnCenterY >> 8));
  }

  u16 region::getIndex() const
  {
    return static_cast<u16>(this - (BW::BWDATA::SAIPathing->regions));
  }

  const BW::region *getRegionAt(int x, int y)
  {
    BWAPI::TilePosition tp(x/32, y/32);
    if ( tp.x < 0 || tp.y < 0 || tp.x >= 256 || tp.y >= 256 )
      return nullptr;

    // Obtain the region IDs from the positions
    u16 id = BW::BWDATA::SAIPathing->mapTileRegionId[tp.y][tp.x];

    // Check if the id is splitting the tile between regions
    if ( id & 0xE000 )
    {
      // Correct the ID (convert higher bits)
      id = (id & 0x1FFF) + ((id & 0xC000) >> 1);

      // Get source region from split-tile based on walk tile
      int minitilePosX = (x&0x1F)/8;
      int minitilePosY = (y&0x1F)/8;
      int minitileShift = minitilePosX + minitilePosY * 4;
      BW::split *t = &BW::BWDATA::SAIPathing->splitTiles[id];
      if ( (t->minitileMask >> minitileShift) & 1 )
        return &BW::BWDATA::SAIPathing->regions[t->rgn2];
      return &BW::BWDATA::SAIPathing->regions[t->rgn1];
    }
    else if ( id < 5000 )
    {
      // Get source region from tile
      return &BW::BWDATA::SAIPathing->regions[id];
    }
    else
    {
      return nullptr;
    }
  }
  const BW::region *getRegionAt(Position pos)
  {
    return getRegionAt(pos.x, pos.y);
  }
};

