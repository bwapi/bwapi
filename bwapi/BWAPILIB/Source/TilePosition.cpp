#include <BWAPI/Constants.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Position.h>

namespace BWAPI
{
  namespace TilePositions
  {
    const TilePosition Invalid(1000,1000);
    const TilePosition None(1000,1001);
    const TilePosition Unknown(1000,1002);
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition()
  :x(0)
  ,y(0)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(int x, int y)
  :x(x)
  ,y(y)
  {
  }
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  TilePosition::TilePosition(const Position& position)
  :x(position.x/TILE_SIZE)
  ,y(position.y/TILE_SIZE)
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
  //----------------------------------------------- IS VALID -------------------------------------------------
  bool TilePosition::isValid() const
  {
    return (*this!=TilePositions::Invalid && *this!=TilePositions::None && *this!=TilePositions::Unknown);
  }
  //----------------------------------------------------------------------------------------------------------
};
