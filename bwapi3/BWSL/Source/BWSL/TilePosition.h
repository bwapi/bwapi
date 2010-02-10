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
      BWSL_EXPORT bool operator == (const TilePosition& TilePosition) const;
      BWSL_EXPORT bool operator != (const TilePosition& TilePosition) const;
      BWSL_EXPORT bool operator  < (const TilePosition& TilePosition) const;
      BWSL_EXPORT TilePosition operator+(const TilePosition& position) const;
      BWSL_EXPORT TilePosition operator-(const TilePosition& position) const;
      BWSL_EXPORT TilePosition& operator+=(const TilePosition& position);
      BWSL_EXPORT TilePosition& operator-=(const TilePosition& position);
      BWSL_EXPORT double getDistance(const TilePosition& position) const;
      BWSL_EXPORT double getLength() const;
      BWSL_EXPORT bool isValid() const;
      BWSL_EXPORT int& x();
      BWSL_EXPORT int& y();
      BWSL_EXPORT int x() const;
      BWSL_EXPORT int y() const;
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

