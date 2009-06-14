#pragma once

namespace BWAPI
{
  class TilePosition;
  class Position
  {
  public :
    Position();
    Position(const TilePosition& position);
    Position(int x, int y);
    bool operator == (const Position& position) const;
    bool operator != (const Position& position) const;
    bool operator  < (const Position& position) const;
    double getDistance(const Position &position) const;
    int x;
    int y;
  };
  namespace Positions
  {
    extern const Position Invalid;
    extern const Position None;
    extern const Position Unknown;
  }
};

