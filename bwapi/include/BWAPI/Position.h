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
      Position operator+(const Position& position) const;
      Position operator-(const Position& position) const;
      Position& operator+=(const Position& position);
      Position& operator-=(const Position& position);
      double getDistance(const Position& position) const;
      double getLength() const;
      int& x();
      int& y();
      int x() const;
      int y() const;
    private :
      int _x;
      int _y;
  };
  namespace Positions
  {
    extern const Position Invalid;
    extern const Position None;
    extern const Position Unknown;
  }
};

