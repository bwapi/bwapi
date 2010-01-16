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
      bool operator == (const Position& position) const;
      bool operator != (const Position& position) const;
      bool operator  < (const Position& position) const;
      Position operator+(const Position& position) const;
      Position operator-(const Position& position) const;
      Position& operator+=(const Position& position);
      Position& operator-=(const Position& position);
      double getDistance(const Position& position) const;
      double getApproxDistance(const Position& position) const;
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
    BWSL_EXPORT extern const Position Invalid;
    BWSL_EXPORT extern const Position None;
    BWSL_EXPORT extern const Position Unknown;
  }
};

