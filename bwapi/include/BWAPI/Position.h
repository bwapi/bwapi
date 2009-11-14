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
      bool __cdecl operator == (const Position& position) const;
      bool __cdecl operator != (const Position& position) const;
      bool __cdecl operator  < (const Position& position) const;
      Position __cdecl operator+(const Position& position) const;
      Position __cdecl operator-(const Position& position) const;
      Position& __cdecl operator+=(const Position& position);
      Position& __cdecl operator-=(const Position& position);
      double __cdecl getDistance(const Position& position) const;
      double __cdecl getLength() const;
      int& __cdecl x();
      int& __cdecl y();
      int __cdecl x() const;
      int __cdecl y() const;
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

