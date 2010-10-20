#pragma once

namespace BWAPI
{
  class Position;

  // TODO: Add doxygen documentation
  class TilePosition
  {
    public :
      TilePosition();
      explicit TilePosition(const Position& position);
      TilePosition(int x, int y);
      bool operator == (const TilePosition& TilePosition) const;
      bool operator != (const TilePosition& TilePosition) const;
      bool operator  < (const TilePosition& TilePosition) const;
      bool isValid() const;
      TilePosition operator+(const TilePosition& position) const;
      TilePosition operator-(const TilePosition& position) const;
      TilePosition& makeValid();
      TilePosition& operator+=(const TilePosition& position);
      TilePosition& operator-=(const TilePosition& position);
      double getDistance(const TilePosition& position) const;
      double getLength() const;
      int& x();
      int& y();
      int x() const;
      int y() const;
    private :
      int _x;
      int _y;
  };
  namespace TilePositions
  {
    extern const TilePosition Invalid;
    extern const TilePosition None;
    extern const TilePosition Unknown;
  }
};

