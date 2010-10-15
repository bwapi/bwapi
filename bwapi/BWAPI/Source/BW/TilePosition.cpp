#include "TilePosition.h"

#include "Position.h"
#include "Offsets.h"

namespace BW
{
  TilePosition TilePosition::Invalid = TilePosition(1000, 1000);
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition()
      : x(0)
      , y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(u16 x, u16 y)
      : x(x)
      , y(y)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(const Position& position)
      : x(position.x / TILE_SIZE)
      , y(position.y / TILE_SIZE)
  {
  }
  //---------------------------------------------- OPERATOR == -----------------------------------------------
  bool TilePosition::operator == (const TilePosition& TilePosition) const
  {
    return this->x == TilePosition.x &&
           this->y == TilePosition.y;
  }
  //---------------------------------------------- OPERATOR != -----------------------------------------------
  bool TilePosition::operator != (const TilePosition& TilePosition) const
  {
    return this->x != TilePosition.x ||
           this->y != TilePosition.y;
  }
  //---------------------------------------------- OPERATOR < ------------------------------------------------
  bool TilePosition::operator < (const TilePosition& TilePosition) const
  {
    return this->x < TilePosition.x ||
           (this->x == TilePosition.x && this->y < TilePosition.y);
  }
  //----------------------------------------------------------------------------------------------------------
  bool TilePosition::isValid()
  {
    return *this != Invalid;
  }
  //----------------------------------------------------------------------------------------------------------
};
