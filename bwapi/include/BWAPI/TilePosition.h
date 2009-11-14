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
      bool __cdecl operator == (const TilePosition& TilePosition) const;
      bool __cdecl operator != (const TilePosition& TilePosition) const;
      bool __cdecl operator  < (const TilePosition& TilePosition) const;
      TilePosition __cdecl operator+(const TilePosition& position) const;
      TilePosition __cdecl operator-(const TilePosition& position) const;
      TilePosition& __cdecl operator+=(const TilePosition& position);
      TilePosition& __cdecl operator-=(const TilePosition& position);
      double __cdecl getDistance(const TilePosition& position) const;
      double __cdecl getLength() const;
      bool __cdecl isValid() const;
      int& __cdecl x();
      int& __cdecl y();
      int __cdecl x() const;
      int __cdecl y() const;
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

