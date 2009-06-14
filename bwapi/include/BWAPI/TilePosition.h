#pragma once

namespace BWAPI
{
  class Position;
  class TilePosition
  {
  public :
    TilePosition();
    TilePosition(const Position& position);
    TilePosition(int x, int y);
    bool operator == (const TilePosition& TilePosition) const;
    bool operator != (const TilePosition& TilePosition) const;
    bool operator  < (const TilePosition& TilePosition) const;
    bool isValid() const;
    int x;
    int y;
  };
  namespace TilePositions
  {
    extern const TilePosition Invalid;
    extern const TilePosition None;
    extern const TilePosition Unknown;
  }
};

