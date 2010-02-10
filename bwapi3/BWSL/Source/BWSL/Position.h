#pragma once

#include "BWSLInterface.h"

namespace BWSL
{
  class TilePosition;
  class Position
  {
    public :
      Position();
      Position(const TilePosition& position);
      Position(int x, int y);
      BWSL_EXPORT bool operator == (const Position& position) const;
      BWSL_EXPORT bool operator != (const Position& position) const;
      BWSL_EXPORT bool operator  < (const Position& position) const;
      BWSL_EXPORT Position operator+(const Position& position) const;
      BWSL_EXPORT Position operator-(const Position& position) const;
      BWSL_EXPORT Position& operator+=(const Position& position);
      BWSL_EXPORT Position& operator-=(const Position& position);
      BWSL_EXPORT double getDistance(const Position& position) const;
      BWSL_EXPORT double getApproxDistance(const Position& position) const;
      BWSL_EXPORT double getLength() const;
      BWSL_EXPORT int& x();
      BWSL_EXPORT int& y();
      BWSL_EXPORT int x() const;
      BWSL_EXPORT int y() const;
    private :
      int _x;
      int _y;
  };
  namespace Positions
  {
    BWSL_EXPORT extern const Position Invalid;
    BWSL_EXPORT extern const Position None;
    BWSL_EXPORT extern const Position Unknown;
  }
};

