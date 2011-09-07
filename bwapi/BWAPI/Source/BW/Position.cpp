#include "Position.h"
#include "TilePosition.h"

#include <math.h>
#include <algorithm>
#include <BWAPI/Constants.h>

#include "Offsets.h"

#include "../../Debug.h"
namespace BW
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position()
      : x(0)
      , y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Position::Position(const BW::TilePosition& position)
      : x(position.x * TILE_SIZE)
      , y(position.y * TILE_SIZE)
  {
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Position::Position(u16 x, u16 y)
      : x(x)
      , y(y)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool Position::operator == (const BW::Position& position) const
  {
    return this->x == position.x &&
           this->y == position.y;
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool Position::operator != (const BW::Position& position) const
  {
    return this->x != position.x ||
           this->y != position.y;
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool Position::operator  < (const BW::Position& position) const
  {
    return this->x   < position.x ||
           (this->x == position.x && this->y < position.y);
  }
  //----------------------------------------------------------------------------------------------------------
  u16 Position::getDistance(const Position& position) const
  {
    return (u16)sqrt((long double)((long double)this->x - position.x) * ((long double)this->x - position.x) +
                     (long double)((long double)this->y - position.y) * ((long double)this->y - position.y));
  }
  //----------------------------------------------------------------------------------------------------------
  int Position::getApproxDistance(const Position& position) const
  {
    unsigned int min = abs(x - position.x);
    unsigned int max = abs(y - position.y);
    if ( max < min )
      std::swap<unsigned int>(min,max);

    if ( min < (max >> 2) )
      return max;

    unsigned int minCalc = (3*min) >> 3;
    return (minCalc >> 5) + minCalc + max - (max >> 4) - (max >> 6);
    // Simplifies to (99*min + 236*max)/256;
  }
  region *Position::getRegion() const
  {
    TilePosition tp(x/32, y/32);
    if ( tp.x >= 256 && tp.y >= 256 )
      return NULL;

    // Obtain the region IDs from the positions
    u16 id = BW::BWDATA_SAIPathing->mapTileRegionId[tp.y][tp.x];

    if ( id & 0x2000 )
    {
      // Get source region from split-tile based on walk tile
      int minitilePosX = (x&0x1F)/8;
      int minitilePosY = (y&0x1F)/8;
      int minitileShift = minitilePosX + minitilePosY * 4;
      BW::split *t = &BW::BWDATA_SAIPathing->splitTiles[id&0x1FFF];
      if ( (t->minitileMask >> minitileShift) & 1 )
        return &BW::BWDATA_SAIPathing->regions[t->rgn2];
      return &BW::BWDATA_SAIPathing->regions[t->rgn1];
    }
    // Get source region from tile
    return &BW::BWDATA_SAIPathing->regions[id];
  }
};
