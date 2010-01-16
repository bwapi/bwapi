#pragma once

#include "BWSLInterface.h"

namespace BWSL
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
      TilePosition operator+(const TilePosition& position) const;
      TilePosition operator-(const TilePosition& position) const;
      TilePosition& operator+=(const TilePosition& position);
      TilePosition& operator-=(const TilePosition& position);
      double getDistance(const TilePosition& position) const;
      double getLength() const;
      bool isValid() const;
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
    BWSL_EXPORT extern const TilePosition Invalid;
    BWSL_EXPORT extern const TilePosition None;
    BWSL_EXPORT extern const TilePosition Unknown;
  }
};

