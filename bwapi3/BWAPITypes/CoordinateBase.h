#pragma once

namespace BWAPI
{
  namespace CoordinateBases
  {
    enum Enum
    {
      Map = 1,
      Screen,
      Mouse
    };
  }
  typedef CoordinateBases::Enum CoordinateBase;
}
